
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                          LIBAPAC CMAKE BUILD FILE                         | 
#   |                                                                           |
#   O---------------------------------------------------------------------------O

# --- CHECKS --------------------------------------------------------------------
    
cmake_minimum_required(VERSION 3.12)
project(libapac)
    
# Check for 64-bit architecture    
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Only 64-bit builds are supported.")
endif()

# Detect platform
if (WIN32)
    message(STATUS "Building on 64-bit Windows")

    if (MINGW)
        message(STATUS "MinGW installation found")

        if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
            message(STATUS "Compiler: GCC (MinGW)")
        elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang")
            message(STATUS "Compiler: Clang (MinGW)")
        else ()
            message(FATAL_ERROR "Unsupported compiler under MinGW: ${CMAKE_C_COMPILER_ID}")
        endif()
    else ()
        message(FATAL_ERROR "Unsupported Windows toolchain (MinGW required)")
    endif()

elseif (APPLE)
    message(STATUS "Building on macOS")

    if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
        message(STATUS "Compiler: Clang (Apple/LLVM)")
    else ()
        message(FATAL_ERROR "Unsupported compiler on macOS: ${CMAKE_C_COMPILER_ID}")
    endif()

elseif (UNIX)
    message(STATUS "Building on Unix/Linux")

    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        message(STATUS "Compiler: GCC")
    elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang")
        message(STATUS "Compiler: Clang")
    else ()
        message(FATAL_ERROR "Unsupported compiler on Unix/Linux: ${CMAKE_C_COMPILER_ID}")
    endif()

else ()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

# For compiling .asm files 
message(STATUS "Enabling Assembly Language (GAS\\LLVM-MC)")
enable_language(ASM)

# Enforce C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
    
# Enable Intel assembly syntax if x86-64
if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    
    message(STATUS "Assembling for Intel Syntax Assembly")    
    add_compile_options(-masm=intel)

    if (WIN32)
        message(STATUS "Assembling for MS-ABI")    
        add_compile_options(
            $<$<COMPILE_LANGUAGE:ASM>:-DAPAC_MSABI>
        )
    
    elseif (APPLE OR UNIX)
        message(STATUS "Assembling for SYSV-ABI")    
        add_compile_options(
            $<$<COMPILE_LANGUAGE:ASM>:-DAPAC_SYSVABI>
        )
    else()
        message(FATAL_ERROR "Unsupported OS for x86-64 assembly ABI")
    endif()

elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|AARCH64")

else()
    message(FATAL_ERROR "Unsupported ISA")
endif()

# --- BUILD OPTIONS -------------------------------------------------------------

# Option to build a shared lib instead of a static lib
option(BUILD_SHARED_LIB "Build libapac as a shared library" OFF)
    
# --- COMMON C SOURCES ----------------------------------------------------------

set(apac_c_sources

    # MISCELLANEOUS

    "src/misc/apacSetMemFuncs.c"
    "src/misc/apacGetCPUSpec.c"
    "src/misc/apacInit.c"

    # APN PUBLIC API

    "src/apn/commons/apn_add.c"
    "src/apn/commons/apn_sub.c"
    "src/apn/commons/apn_negate.c"
    "src/apn/commons/apn_mul.c"
    "src/apn/commons/apn_set.c"
    "src/apn/commons/apn_cpy.c"
    "src/apn/commons/apn_sqr.c"

    # APN HIDDEN FUNCS

    "src/apn/commons/basecase_mul.c"
    "src/apn/commons/karatsuba_mul.c"
    "src/apn/commons/basecase_sqr.c"
    "src/apn/commons/karatsuba_sqr.c"

)
    
# --- x64 SPECIFIC C SOURCES ----------------------------------------------------

if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")

    set(x64_specific

        # C SOURCES

        "src/apn/x64/set.c"
        "src/apn/x64/cpy.c"
        "src/apn/x64/cmp.c"
        "src/apn/x64/cpu_params.c"

        # ASSEMBLY SOURCES

        "src/apn/x64/add_n.asm"
        "src/apn/x64/add_n_one.asm"
        "src/apn/x64/add_n_one_till_carry.asm"    
        "src/apn/x64/mul_bc.asm"
        "src/apn/x64/addmul_one.asm"
        "src/apn/x64/sqr_bc.asm"
        "src/apn/x64/sub_n.asm"
        "src/apn/x64/sub_n_one.asm"
        "src/apn/x64/sub_n_one_till_borrow.asm"
        "src/apn/x64/neg.asm"

    )

elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|AARCH64")

endif()
    
# --- HIDDEN HEADERS ------------------------------------------------------------

set(hidden_headers

    # COMMONS

    "src/apn/commons/hidden_mul.h"
    "src/apn/commons/hidden_sqr.h"
    "src/apn/commons/apn_thresholds.h"
    
    # x64 SPECIFIC

    "src/apn/x64/x64_hidden_funcs.h"

    # AArch64 SPECIFIC


)
    
# --- BUILDING DLL/STATIC -------------------------------------------------------

if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")

    set(isa_sources ${x64_specific})

    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "AARCH64|aarch64")

endif()

# --- Combine all source files ---
set(apac_sources ${apac_c_sources} ${isa_sources} ${hidden_headers})

# Build Shared/Static Library
    
if(BUILD_SHARED_LIB)
    add_library(apac SHARED ${apac_sources})
    target_compile_definitions(apac PRIVATE BUILD_SHARED_LIB)
else()
    add_library(apac STATIC ${apac_sources})
endif()

set(apac_target apac)
    
# Configuration-specific definitions

target_compile_definitions(${apac_target} PRIVATE
    "$<$<CONFIG:Release>:APAC_DISABLE_ASSERT>"
)