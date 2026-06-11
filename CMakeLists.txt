#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                          LIBAPAC CMAKE BUILD FILE                         |
#   |                                                                           |
#   O---------------------------------------------------------------------------O

# --- CHECKS --------------------------------------------------------------------

cmake_minimum_required(VERSION 3.12)
project(apac)

enable_testing()

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

option(BUILD_SHARED_LIB "Build libapac as a shared library"                             OFF)
option(BUILD_APAC_TESTS "Build apn_tests program for testing correctness of routines"   OFF)
option(BUILD_APN_TUNE   "Build apn_tune threshold tuning program"                       OFF)
option(BUILD_APAC_MT    "Build the multithreaded routines in libapac"                   OFF)

# --- COMMON C SOURCES ----------------------------------------------------------

set(apac_c_sources

    # MISCELLANEOUS

    "src/misc/apac_init.c"
    "src/misc/apac_init_alloc.c"
    "src/misc/apac_get_cpu_spec.c"
    "src/misc/apac_utilities.c"

    # APN PUBLIC API

    "src/apn/commons/apn_add.c"
    "src/apn/commons/apn_sub.c"
    "src/apn/commons/apn_negate.c"
    "src/apn/commons/apn_mul.c"
    "src/apn/commons/apn_set.c"
    "src/apn/commons/apn_cpy.c"
    "src/apn/commons/apn_sqr.c"
    "src/apn/commons/apn_cmp.c"
    "src/apn/commons/apn_mul_one.c"
    "src/apn/commons/apn_addmul_one.c"
    "src/apn/commons/apn_submul_one.c"
    "src/apn/commons/apn_shift.c"
    "src/apn/commons/apn_is_zero.c"
    "src/apn/commons/apn_clamp.c"
    "src/apn/commons/apn_div.c"
    "src/apn/commons/apn_div_one.c"
    "src/apn/commons/apn_set_random.c"

    # APN HIDDEN FUNCS

    "src/apn/commons/basecase_div.c"
    "src/apn/commons/basecase_sqr.c"
    "src/apn/commons/basecase_mul.c"
    "src/apn/commons/dnc_div.c"
    "src/apn/commons/karatsuba_sqr.c"
    "src/apn/commons/karatsuba_mul.c"
    "src/apn/commons/div_helpers.c"
    "src/apn/commons/reduce_mod_p.c"

    # APZ PUBLIC API

    "src/apz/apz_init_zero.c"
    "src/apz/apz_init_random.c"
    "src/apz/apz_init_one.c"
    "src/apz/apz_init_from_str.c"
    "src/apz/apz_resize.c"
    "src/apz/apz_free.c"
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
            "src/apn/x64/asm_src/ms_abi/add_sub_n.asm"
            "src/apn/x64/asm_src/ms_abi/add_sub_one.asm"
            "src/apn/x64/asm_src/ms_abi/mul_bc.asm"
            "src/apn/x64/asm_src/ms_abi/mul_one.asm"
            "src/apn/x64/asm_src/ms_abi/addmul_one.asm"
            "src/apn/x64/asm_src/ms_abi/submul_one.asm"
            "src/apn/x64/asm_src/ms_abi/sqr_bc.asm"
            "src/apn/x64/asm_src/ms_abi/neg.asm"
            "src/apn/x64/asm_src/ms_abi/lshift_lt64.asm"
            "src/apn/x64/asm_src/ms_abi/rshift_lt64.asm"
        )
    
    elseif (APPLE OR UNIX)

        message(STATUS "Using SYSV-ABI x64 assembly sources")
        set(x64_asm_sources

            "src/apn/x64/asm_src/sysv_abi/add_sub_n.s"
            "src/apn/x64/asm_src/sysv_abi/add_sub_one.s"
            "src/apn/x64/asm_src/sysv_abi/mul_bc.s"
            "src/apn/x64/asm_src/sysv_abi/mul_one.s"
            "src/apn/x64/asm_src/sysv_abi/addmul_one.s"
            "src/apn/x64/asm_src/sysv_abi/submul_one.s"
            "src/apn/x64/asm_src/sysv_abi/sqr_bc.s"
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

# --- MULTI-THREADED C SOURCES --------------------------------------------------

set(apac_mt_c_sources)

if(BUILD_APAC_MT)

    list(APPEND apac_mt_c_sources

        "src/multithreading/parallel_for.c"
    )

endif()

# --- BUILDING DLL/STATIC -------------------------------------------------------

if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")

    set(isa_sources ${x64_specific})

elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "AARCH64|aarch64")

endif()

set(apac_sources ${apac_c_sources} ${isa_sources} ${apac_mt_c_sources})

# Build Shared/Static Library
if(BUILD_SHARED_LIB)
    add_library(apac SHARED ${apac_sources})

    target_compile_definitions(apac PRIVATE 
        $<$<COMPILE_LANGUAGE:C>:BUILD_SHARED_LIB>
    )
    
else()
    add_library(apac STATIC ${apac_sources})
endif()

set(apac_target apac)

target_compile_definitions(${apac_target} PRIVATE
    "$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:APAC_DISABLE_ASSERT>"
)

# --- STRIP PATHS FROM __FILE__ -------------------------------------------------

if (MSVC AND NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
    # MSVC (cl.exe)
    target_compile_options(${apac_target} PRIVATE
        $<$<COMPILE_LANGUAGE:C>:/d1trimfile:${CMAKE_SOURCE_DIR}/>
    )

elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang" AND MSVC)
    # clang-cl
    target_compile_options(${apac_target} PRIVATE
        $<$<COMPILE_LANGUAGE:C>:/clang:-ffile-prefix-map=${CMAKE_SOURCE_DIR}/=>
    )

else()
    # GCC / Clang
    target_compile_options(${apac_target} PRIVATE
        $<$<COMPILE_LANGUAGE:C>:-ffile-prefix-map=${CMAKE_SOURCE_DIR}/=>
    )
endif()

# --- APAC TESTS ----------------------------------------------------------------

if (BUILD_APAC_TESTS)

    message(STATUS "Building APAC tests")

    set(APAC_TEST_SOURCES

        "tests/test_apn_div.c"
        "tests/test_apn_mul.c"
        "tests/test_apn_mul_n.c"
        "tests/test_apn_sqr.c"
        "tests/test_apn_set.c"    
        "tests/test_apn_cpy.c"
        "tests/test_apn_cmp.c"
        "tests/test_apn_is_zero.c"
        "tests/test_apn_add_one.c"
        "tests/test_apn_add_n.c"
        "tests/test_apn_add.c"
        "tests/test_apn_neg.c"
        "tests/test_apn_sub_one.c"
        "tests/test_apn_sub_n.c"
        "tests/test_apn_sub.c"
        "tests/test_apn_mul_one.c"
        "tests/test_apn_addmul_one.c"
        "tests/test_apn_submul_one.c"
        "tests/test_apn_div_one.c"
        "tests/test_apn_lshift.c"
        "tests/test_apn_rshift.c"
    )

    foreach(test_src ${APAC_TEST_SOURCES})

        get_filename_component(test_name ${test_src} NAME_WE)

        add_executable(${test_name}
            ${test_src}
        )

        target_link_libraries(${test_name} PRIVATE
            ${apac_target}
        )

        target_include_directories(${test_name} PRIVATE
            ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/src/header
        )

        add_test(
            NAME ${test_name}
            COMMAND ${test_name}
        )

    endforeach()

endif()

# --- APN TUNING ----------------------------------------------------------------

if (BUILD_APN_TUNE)

    message(STATUS "Building apn_tune")

    add_executable(apn_tune
        "tuning/apn_tune.c"
    )

    target_link_libraries(apn_tune PRIVATE ${apac_target})

    target_include_directories(apn_tune PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

endif()