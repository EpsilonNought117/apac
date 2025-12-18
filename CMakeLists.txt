
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                          LIBAPAC CMAKE BUILD FILE                         |
#   |                                                                           |
#   O---------------------------------------------------------------------------O

# --- CHECKS --------------------------------------------------------------------

cmake_minimum_required(VERSION 3.12)
project(libapac)

# Requires a 64-bit Computer
if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Only 64-bit builds are supported.")
endif()

# Detect platform & compiler
if (WIN32)
    message(STATUS "Building on 64-bit Windows")

    if (MSVC)
        if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
            message(STATUS "Compiler: Clang-cl (version ${CMAKE_C_COMPILER_VERSION})")

        else ()
            message(STATUS "Compiler: MSVC (version ${MSVC_VERSION})")

        endif()

    else ()
        message(FATAL_ERROR "Unsupported compiler on Windows: ${CMAKE_C_COMPILER_ID}")

    endif()

    if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
        message(STATUS "Enabling MASM64 assembler")
        enable_language(ASM_MASM)

    endif()

elseif (APPLE)
    message(STATUS "Building on macOS")

    if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
        message(STATUS "Compiler: Clang")

    else ()
        message(FATAL_ERROR "Unsupported compiler on macOS: ${CMAKE_C_COMPILER_ID}")

    endif()

    message(STATUS "Enabling assembler")
    enable_language(ASM)

elseif (UNIX)
    message(STATUS "Building on Unix/Linux")

    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        message(STATUS "Compiler: GCC")

    elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang")
        message(STATUS "Compiler: Clang")

    else ()
        message(FATAL_ERROR "Unsupported compiler on Unix/Linux: ${CMAKE_C_COMPILER_ID}")

    endif()

    message(STATUS "Enabling assembler")
    enable_language(ASM)

else ()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")

endif()

# Enforce C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

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
    "src/apn/commons/apn_cmp.c"
    "src/apn/commons/apn_addmul_one.c"
    "src/apn/commons/apn_submul_one.c"
    "src/apn/commons/apn_shift.c"
    "src/apn/commons/apn_is_zero.c"
    "src/apn/commons/apn_clamp.c"
    "src/apn/commons/apn_div.c"
    "src/apn/commons/apn_div_one.c"

    # APN HIDDEN FUNCS

    "src/apn/commons/basecase_div.c"
    "src/apn/commons/dnc_div_rem.c"
    "src/apn/commons/basecase_mul.c"
    "src/apn/commons/karatsuba_mul.c"
    "src/apn/commons/basecase_sqr.c"
    "src/apn/commons/karatsuba_sqr.c"
    "src/apn/commons/div_helpers.c"
)
    
# --- x64 SPECIFIC SOURCES ------------------------------------------------------
if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")

    # Common C sources (always used on x64)
    set(x64_c_sources
        "src/apn/x64/set.c"
        "src/apn/x64/cmp.c"
        "src/apn/x64/cpy.c"
        "src/apn/x64/is_zero.c"
        "src/apn/x64/cpu_params.c"
    )

    # ABI-specific assembly sources
    if (WIN32)
        message(STATUS "Using MS-ABI x64 assembly sources")
        set(x64_asm_sources
            "src/apn/x64/ms_abi/add_n.asm"
            "src/apn/x64/ms_abi/add_n_one.asm"
            "src/apn/x64/ms_abi/mul_bc.asm"
            "src/apn/x64/ms_abi/addmul_one.asm"
            "src/apn/x64/ms_abi/submul_one.asm"
            "src/apn/x64/ms_abi/sqr_bc.asm"
            "src/apn/x64/ms_abi/sub_n.asm"
            "src/apn/x64/ms_abi/sub_n_one.asm"
            "src/apn/x64/ms_abi/neg.asm"
            "src/apn/x64/ms_abi/lshift_lt64.asm"
            "src/apn/x64/ms_abi/rshift_lt64.asm"
        )
    elseif (APPLE OR UNIX)
        message(STATUS "Using SYSV-ABI x64 assembly sources")
        set(x64_asm_sources
            "src/apn/x64/ms_abi/add_n.asm"
            "src/apn/x64/ms_abi/add_n_one.asm"
            "src/apn/x64/ms_abi/mul_bc.asm"
            "src/apn/x64/ms_abi/addmul_one.asm"
            "src/apn/x64/ms_abi/submul_one.asm"
            "src/apn/x64/ms_abi/sqr_bc.asm"
            "src/apn/x64/ms_abi/sub_n.asm"
            "src/apn/x64/ms_abi/sub_n_one.asm"
            "src/apn/x64/ms_abi/neg.asm"
            "src/apn/x64/ms_abi/lshift_lt64.asm"
            "src/apn/x64/ms_abi/rshift_lt64.asm"
        )
    else ()
        message(FATAL_ERROR "Unsupported OS for x64 assembly ABI")
    endif()

    set(x64_specific ${x64_c_sources} ${x64_asm_sources})

# --- AArch64 SPECIFIC SOURCES --------------------------------------------------

elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|AARCH64")

endif()

# --- HIDDEN HEADERS ------------------------------------------------------------
set(hidden_headers
    # COMMONS
    "src/apn/commons/hidden_mul.h"
    "src/apn/commons/hidden_sqr.h"
    "src/apn/commons/hidden_div.h"
    "src/apn/commons/hidden_helpers.h"
    "src/apn/commons/apn_thresholds.h"

    # x64 SPECIFIC
    "src/apn/x64/x64_hidden_funcs.h"

    # AArch64 SPECIFIC
)

# --- STRIP PATHS FROM __FILE__ -------------------------------------------------\

foreach(src_file ${apac_sources})
    get_filename_component(file_name ${src_file} NAME)
    set_source_files_properties(
        ${src_file}
        PROPERTIES
        COMPILE_DEFINITIONS "__FILE__=\"${file_name}\""
    )
endforeach()

# --- BUILDING DLL/STATIC -------------------------------------------------------

if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    set(isa_sources ${x64_specific})
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "AARCH64|aarch64")
    
endif()

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