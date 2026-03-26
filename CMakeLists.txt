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
option(BUILD_APN_TESTS "Build apn_tests program for testing correctness of routines" ON)
option(BUILD_APN_TUNE "Build apn_tune threshold tuning program" ON)
option(BUILD_EXAMPLES "Build programming examples to demonstrate working of the library" ON)

# --- COMMON C SOURCES ----------------------------------------------------------

set(apac_c_sources

    # MISCELLANEOUS

    "src/misc/apac_set_mem_funcs.c"
    "src/misc/apac_get_cpu_spec.c"
    "src/misc/apac_init.c"

    # APN PUBLIC API

    "src/apn/commons/c_src/apn_add.c"
    "src/apn/commons/c_src/apn_sub.c"
    "src/apn/commons/c_src/apn_negate.c"
    "src/apn/commons/c_src/apn_mul.c"
    "src/apn/commons/c_src/apn_set.c"
    "src/apn/commons/c_src/apn_cpy.c"
    "src/apn/commons/c_src/apn_sqr.c"
    "src/apn/commons/c_src/apn_cmp.c"
    "src/apn/commons/c_src/apn_addmul_one.c"
    "src/apn/commons/c_src/apn_submul_one.c"
    "src/apn/commons/c_src/apn_shift.c"
    "src/apn/commons/c_src/apn_is_zero.c"
    "src/apn/commons/c_src/apn_clamp.c"
    "src/apn/commons/c_src/apn_div.c"
    "src/apn/commons/c_src/apn_div_one.c"

    # APN HIDDEN FUNCS

    "src/apn/commons/c_src/basecase_div.c"
    "src/apn/commons/c_src/dnc_div.c"
    "src/apn/commons/c_src/basecase_mul.c"
    "src/apn/commons/c_src/karatsuba_mul.c"
    "src/apn/commons/c_src/basecase_sqr.c"
    "src/apn/commons/c_src/karatsuba_sqr.c"
    "src/apn/commons/c_src/div_helpers.c"
)

# --- x64 SPECIFIC SOURCES ------------------------------------------------------
if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")

    # Common C sources (always used on x64)
    set(x64_c_sources
        "src/apn/x64/c_src/set.c"
        "src/apn/x64/c_src/cmp.c"
        "src/apn/x64/c_src/cpy.c"
        "src/apn/x64/c_src/is_zero.c"
        "src/apn/x64/c_src/cpu_params.c"
    )

    # ABI-specific assembly sources
    if (WIN32)
        message(STATUS "Using MS-ABI x64 assembly sources")
        set(x64_asm_sources
            "src/apn/x64/asm_src/ms_abi/add_n.asm"
            "src/apn/x64/asm_src/ms_abi/add_one.asm"
            "src/apn/x64/asm_src/ms_abi/mul_bc.asm"
            "src/apn/x64/asm_src/ms_abi/addmul_one.asm"
            "src/apn/x64/asm_src/ms_abi/submul_one.asm"
            "src/apn/x64/asm_src/ms_abi/sqr_bc.asm"
            "src/apn/x64/asm_src/ms_abi/sub_n.asm"
            "src/apn/x64/asm_src/ms_abi/sub_one.asm"
            "src/apn/x64/asm_src/ms_abi/neg.asm"
            "src/apn/x64/asm_src/ms_abi/lshift_lt64.asm"
            "src/apn/x64/asm_src/ms_abi/rshift_lt64.asm"
        )
    elseif (APPLE OR UNIX)
        message(STATUS "Using SYSV-ABI x64 assembly sources")
        set(x64_asm_sources
            "src/apn/x64/asm_src/sysv_abi/add_n.s"
            "src/apn/x64/asm_src/sysv_abi/add_one.s"
            "src/apn/x64/asm_src/sysv_abi/mul_bc.s"
            "src/apn/x64/asm_src/sysv_abi/addmul_one.s"
            "src/apn/x64/asm_src/sysv_abi/submul_one.s"
            "src/apn/x64/asm_src/sysv_abi/sqr_bc.s"
            "src/apn/x64/asm_src/sysv_abi/sub_n.s"
            "src/apn/x64/asm_src/sysv_abi/sub_one.s"
            "src/apn/x64/asm_src/sysv_abi/neg.s"
            "src/apn/x64/asm_src/sysv_abi/lshift_lt64.s"
            "src/apn/x64/asm_src/sysv_abi/rshift_lt64.s"
        )
    else ()
        message(FATAL_ERROR "Unsupported OS for x64 assembly ABI")
    endif()

    set(x64_specific "src/apn/x64/x64_hidden_funcs.h" ${x64_c_sources} ${x64_asm_sources})

# --- AArch64 SPECIFIC SOURCES --------------------------------------------------

elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|AARCH64")

endif()

# --- HIDDEN HEADERS ------------------------------------------------------------

set(hidden_headers
    # COMMONS
    "src/apn/commons/c_headers/hidden_sqr.h"
    "src/apn/commons/c_headers/hidden_mul.h"
    "src/apn/commons/c_headers/hidden_div.h"
    "src/apn/commons/c_headers/hidden_helpers.h"
    "src/apn/commons/c_headers/apn_thresholds.h"
)

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

# --- STRIP PATHS FROM __FILE__ -------------------------------------------------

if (MSVC AND NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
    # MSVC (cl.exe)
    target_compile_options(${apac_target} PRIVATE
        $<$<COMPILE_LANGUAGE:C>:/d1trimfile:${CMAKE_SOURCE_DIR}/>
    )

elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang" AND MSVC)
    # clang-cl (Clang with MSVC style args)
    target_compile_options(${apac_target} PRIVATE
        /clang:-ffile-prefix-map=${CMAKE_SOURCE_DIR}/=
    )

else()
    # GCC / Clang (GNU-style)
    target_compile_options(${apac_target} PRIVATE 
        -ffile-prefix-map=${CMAKE_SOURCE_DIR}/=
    )
endif()

# --- APN TESTS -----------------------------------------------------------------

if (BUILD_APN_TESTS)

    message(STATUS "Building apn_tests")

    add_executable(apn_tests
        "tests/apn_tests.c"
        "utilities/apac_utilities.c"
    )

    target_link_libraries(apn_tests PRIVATE ${apac_target})

    target_include_directories(apn_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include     # for apac.h
        ${CMAKE_SOURCE_DIR}/utilities   # for apac_utilities.h
    )

endif()

# --- APN TUNING ----------------------------------------------------------------

if (BUILD_APN_TUNE)

    message(STATUS "Building apn_tune")

    add_executable(apn_tune
        "tuning/apn_tune.c"
        "utilities/apac_utilities.c"
    )

    target_link_libraries(apn_tune PRIVATE ${apac_target})

    target_include_directories(apn_tune PRIVATE
        ${CMAKE_SOURCE_DIR}/include     # for apac.h
        ${CMAKE_SOURCE_DIR}/utilities   # for apac_utilities.h
    )

endif()

# --- EXAMPLES -----------------------------------------------------------------

if (BUILD_EXAMPLES)

    message(STATUS "Building examples")

    set(APAC_EXAMPLES
        factorial
        fibonacci
    )

    foreach(exe IN LISTS APAC_EXAMPLES)
        add_executable(${exe} examples/${exe}.c)

        target_link_libraries(${exe} PRIVATE ${apac_target})

        target_include_directories(${exe} PRIVATE
            ${CMAKE_SOURCE_DIR}/include
        )

        if (UNIX AND NOT APPLE)
            target_link_libraries(${exe} PRIVATE m)
        endif()
    endforeach()

endif()
