
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |            APAC - Arbitrary Precision Arithmetic and Computation          |
#   |                                                                           |
#   O---------------------------------------------------------------------------O

# --- CHECKS --------------------------------------------------------------------

cmake_minimum_required(VERSION 3.26)
project(apac)

enable_testing()

# Requires a 64-bit target architecture
if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)

    message(FATAL_ERROR "This project requires a 64-bit target architecture. "
                        "Detected a ${CMAKE_SIZEOF_VOID_P}-byte pointer width, "
                        "indicating a 32-bit target. 32-bit builds are not supported.")

endif()

# detect CPU ISA

if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64|x64)$")
    set(APAC_CMAKE_X64 TRUE)
else()
    set(APAC_CMAKE_X64 FALSE)
endif()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|ARM64|arm64)$")
    set(APAC_CMAKE_ARM64 TRUE)
else()
    set(APAC_CMAKE_ARM64 FALSE)
endif()

# Detect platform and compiler

if (WIN32)

    message(STATUS "Target platform: Windows")

    if (MINGW)

        message(FATAL_ERROR "Unsupported toolchain on Windows: MinGW (GCC/Clang). "
                            "Supported toolchains are MSVC, Clang-cl, and Clang targeting the MSVC ABI.")

    endif()

    if (MSVC)

        if (CMAKE_C_COMPILER_ID STREQUAL "Clang")

            message(STATUS "Selected C11 compiler: Clang-cl (version ${CMAKE_C_COMPILER_VERSION})")

        else ()

            message(STATUS "Selected C11 compiler: MSVC (version ${CMAKE_C_COMPILER_VERSION})")

        endif()

    elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang")

        message(STATUS "Selected C11 compiler: Clang (version ${CMAKE_C_COMPILER_VERSION}), targeting the MSVC ABI")

    else ()

        message(FATAL_ERROR "Unsupported C11 compiler on Windows: ${CMAKE_C_COMPILER_ID}. "
                            "Supported compilers are MSVC, Clang-cl, and Clang.")

    endif()

    if (APAC_CMAKE_X64)

        message(STATUS "Target processor ISA: x86-64/AMD64")
        enable_language(ASM_MASM)

    elseif (APAC_CMAKE_ARM64)

        message(STATUS "Target processor ISA: AArch64/ARM64")
        enable_language(ASM_MARMASM)

    else ()

        message(FATAL_ERROR "Unsupported processor ISA on Windows: ${CMAKE_SYSTEM_PROCESSOR}. "
                            "Supported ISAs are x86-64/AMD64 and AArch64/ARM64.")

    endif()

elseif (APPLE)

    message(STATUS "Target platform: macOS")

    if (CMAKE_C_COMPILER_ID STREQUAL "AppleClang")

        message(STATUS "Selected C11 compiler: Apple Clang (version ${CMAKE_C_COMPILER_VERSION})")

    else ()

        message(FATAL_ERROR "Unsupported C11 compiler on macOS: ${CMAKE_C_COMPILER_ID}. "
                            "Only Apple Clang is supported.")

    endif()

    if (APAC_CMAKE_X64 OR APAC_CMAKE_ARM64)

        message(STATUS "Target processor ISA: ${CMAKE_SYSTEM_PROCESSOR}")
        enable_language(ASM)

    else ()

        message(FATAL_ERROR "Unsupported processor ISA on macOS: ${CMAKE_SYSTEM_PROCESSOR}. "
                            "Supported ISAs are x86-64 and ARM64.")

    endif()

elseif (LINUX)

    message(STATUS "Target platform: Linux")

    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")

        message(STATUS "Selected C11 compiler: GCC (version ${CMAKE_C_COMPILER_VERSION})")

    elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang")

        message(STATUS "Selected C11 compiler: Clang (version ${CMAKE_C_COMPILER_VERSION})")

    else ()

        message(FATAL_ERROR "Unsupported C11 compiler on Linux: ${CMAKE_C_COMPILER_ID}. "
                            "Supported compilers are GCC and Clang.")

    endif()

    if (APAC_CMAKE_X64 OR APAC_CMAKE_ARM64)

        message(STATUS "Target processor ISA: ${CMAKE_SYSTEM_PROCESSOR}")
        enable_language(ASM)

    else ()

        message(FATAL_ERROR "Unsupported processor ISA on Linux: ${CMAKE_SYSTEM_PROCESSOR}. "
                            "Supported ISAs are x86-64 and AArch64/ARM64.")

    endif()

else ()

    message(FATAL_ERROR "Unsupported target platform. "
                        "This project supports Windows, macOS, and Linux only.")

endif()

# --- COMPILER ARGUMENTS ---------------------------------------------------------

if (MSVC AND NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")

    # MSVC (cl.exe)
    add_compile_options(
        $<$<COMPILE_LANGUAGE:C>:/d1trimfile:${CMAKE_SOURCE_DIR}/>
        $<$<COMPILE_LANGUAGE:C>:/std:c11>
        $<$<COMPILE_LANGUAGE:C>:/experimental:c11atomics>
    )

elseif (MSVC AND CMAKE_C_COMPILER_ID STREQUAL "Clang")

    # clang-cl (MSVC-compatible driver)
    add_compile_options(
        $<$<COMPILE_LANGUAGE:C>:/clang:-ffile-prefix-map=${CMAKE_SOURCE_DIR}/=>
        $<$<COMPILE_LANGUAGE:C>:/std:c11>
        $<$<COMPILE_LANGUAGE:C>:/experimental:c11atomics>
    )

elseif (WIN32 AND CMAKE_C_COMPILER_ID STREQUAL "Clang")

    # Clang targeting the MSVC ABI, native GNU-style driver (not clang-cl)
    add_compile_options(
        $<$<COMPILE_LANGUAGE:C>:-ffile-prefix-map=${CMAKE_SOURCE_DIR}/=>
        $<$<COMPILE_LANGUAGE:C>:-std=c11>
    )

else()

    # GCC / Clang (Linux, macOS)
    add_compile_options(
        $<$<COMPILE_LANGUAGE:C>:-ffile-prefix-map=${CMAKE_SOURCE_DIR}/=>
        $<$<COMPILE_LANGUAGE:C>:-std=c11>
    )

endif()

# --- BUILD OPTIONS -------------------------------------------------------------

option(APAC_SHARED   "Build apac as a shared library"   OFF)
option(APAC_TESTS    "Build property tests"             OFF)
option(APAC_TUNE     "Build performance tuning utility" OFF)
option(APAC_MT_FUNCS "Build multi-threaded functions"   OFF)

# --- Single-Threaded C Sources -------------------------------------------------

set(st_c_sources

    # MISCELLANEOUS

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
)

# --- Multi-Threaded C Sources --------------------------------------------------

set (mt_c_sources

)

# --- x64 specific sources ------------------------------------------------------

if (APAC_CMAKE_X64)

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

        message(STATUS "Using MS-ABI x86-64 assembly sources")

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
    
    elseif (APPLE OR LINUX)

        message(STATUS "Using SysV-ABI x86-64 assembly sources")

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

    endif()

    set(isa_sources "src/apn/x64/x64_hidden_funcs.h" ${x64_c_sources} ${x64_asm_sources})

# --- AArch64/ARM64 specific sources --------------------------------------------------

elseif (APAC_CMAKE_ARM64)

endif()

# --- BUILDING DLL/STATIC -------------------------------------------------------

set(apac_sources ${mt_c_sources} ${st_c_sources} ${isa_sources})

if(APAC_SHARED)

    add_library(apac SHARED ${apac_sources})

    target_compile_definitions(apac PRIVATE $<$<COMPILE_LANGUAGE:C>:APAC_BUILD_SHARED>)
    
else()

    add_library(apac STATIC ${apac_sources})

endif()

set(apac_target apac)

target_compile_definitions(${apac_target} PRIVATE
    "$<$<AND:$<COMPILE_LANGUAGE:C>,$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>>:APAC_DISABLE_ASSERT>"
)

target_compile_definitions(${apac_target} PRIVATE
    "$<$<AND:$<BOOL:${APAC_MT_FUNCS}>,$<COMPILE_LANGUAGE:C>>:APAC_MT_FUNCS>"
)

# --- APAC TESTS ----------------------------------------------------------------

if (APAC_TESTS)

    message(STATUS "Building APAC tests")

    set(APAC_TEST_SOURCES

        "tests/test_apn_set.c"
        "tests/test_apn_cpy.c"
        "tests/test_apn_cmp.c"
        "tests/test_apn_is_zero.c"

        "tests/test_apn_add_one.c"
        "tests/test_apn_add_n.c"
        "tests/test_apn_add.c"

        "tests/test_apn_sub_one.c"
        "tests/test_apn_sub_n.c"
        "tests/test_apn_sub.c"
        
        "tests/test_apn_neg.c"

        "tests/test_apn_lshift.c"
        "tests/test_apn_rshift.c"
    
        "tests/test_apn_mul_one.c"
        "tests/test_apn_addmul_one.c"
        "tests/test_apn_submul_one.c"
        "tests/test_apn_div_one.c"
        
        "tests/test_apn_mul_n.c"
        "tests/test_apn_sqr.c"
        "tests/test_apn_mul.c"    
        "tests/test_apn_div.c"
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

if (APAC_TUNE)

    message(STATUS "Building tuning utility")

    add_executable(apn_tune
        "tuning/apn_tune.c"
    )

    target_link_libraries(apn_tune PRIVATE ${apac_target})

    target_include_directories(apn_tune PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

endif()