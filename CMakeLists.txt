#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                          LIBAPAC CMAKE BUILD FILE                         |
#   |                                                                           |
#   O---------------------------------------------------------------------------O

cmake_minimum_required(VERSION 3.12)
project(libapac LANGUAGES C)

# -------------------------------------------------------------------------------
# ARCH / PLATFORM CHECKS
# -------------------------------------------------------------------------------

if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Only 64-bit builds are supported.")
endif()

if (WIN32)
    message(STATUS "Building on 64-bit Windows")

    if (MSVC)
        if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
            message(STATUS "Compiler: Clang-cl (${CMAKE_C_COMPILER_VERSION})")
        else()
            message(STATUS "Compiler: MSVC (${MSVC_VERSION})")
        endif()
    else()
        message(FATAL_ERROR "Unsupported compiler on Windows")
    endif()

    if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
        enable_language(ASM_MASM)
    endif()

elseif (APPLE OR UNIX)
    message(STATUS "Building on Unix-like OS")

    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        message(STATUS "Compiler: GCC")
    elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang")
        message(STATUS "Compiler: Clang")
    else()
        message(FATAL_ERROR "Unsupported compiler")
    endif()

    enable_language(ASM)

else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# -------------------------------------------------------------------------------
# C STANDARD
# -------------------------------------------------------------------------------

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# -------------------------------------------------------------------------------
# BUILD OPTIONS
# -------------------------------------------------------------------------------

option(BUILD_SHARED_LIB "Build libapac as a shared library" OFF)

# -------------------------------------------------------------------------------
# COMMON C SOURCES
# -------------------------------------------------------------------------------

set(apac_c_sources
    src/misc/apacSetMemFuncs.c
    src/misc/apacGetCPUSpec.c
    src/misc/apacInit.c

    src/apn/commons/apn_add.c
    src/apn/commons/apn_sub.c
    src/apn/commons/apn_negate.c
    src/apn/commons/apn_mul.c
    src/apn/commons/apn_set.c
    src/apn/commons/apn_cpy.c
    src/apn/commons/apn_sqr.c
    src/apn/commons/apn_cmp.c
    src/apn/commons/apn_addmul_one.c
    src/apn/commons/apn_submul_one.c
    src/apn/commons/apn_shift.c
    src/apn/commons/apn_is_zero.c
    src/apn/commons/apn_clamp.c
    src/apn/commons/apn_div.c
    src/apn/commons/apn_div_one.c

    src/apn/commons/basecase_div.c
    src/apn/commons/dnc_div_rem.c
    src/apn/commons/basecase_mul.c
    src/apn/commons/karatsuba_mul.c
    src/apn/commons/basecase_sqr.c
    src/apn/commons/karatsuba_sqr.c
    src/apn/commons/div_helpers.c
)

# -------------------------------------------------------------------------------
# x64 SOURCES
# -------------------------------------------------------------------------------

if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")

    set(x64_c_sources
        src/apn/x64/set.c
        src/apn/x64/cmp.c
        src/apn/x64/cpy.c
        src/apn/x64/is_zero.c
        src/apn/x64/cpu_params.c
    )

    if (WIN32)
        message(STATUS "Using MS-ABI x64 assembly sources")
        set(x64_asm_sources
            src/apn/x64/ms_abi/add_n.asm
            src/apn/x64/ms_abi/add_one.asm
            src/apn/x64/ms_abi/mul_bc.asm
            src/apn/x64/ms_abi/addmul_one.asm
            src/apn/x64/ms_abi/submul_one.asm
            src/apn/x64/ms_abi/sqr_bc.asm
            src/apn/x64/ms_abi/sub_n.asm
            src/apn/x64/ms_abi/sub_one.asm
            src/apn/x64/ms_abi/neg.asm
            src/apn/x64/ms_abi/lshift_lt64.asm
            src/apn/x64/ms_abi/rshift_lt64.asm
        )
    else()
        message(STATUS "Using SYSV-ABI x64 assembly sources")
        set(x64_asm_sources
            src/apn/x64/sysv_abi/add_n.s
            src/apn/x64/sysv_abi/add_one.s
            src/apn/x64/sysv_abi/mul_bc.s
            src/apn/x64/sysv_abi/addmul_one.s
            src/apn/x64/sysv_abi/submul_one.s
            src/apn/x64/sysv_abi/sqr_bc.s
            src/apn/x64/sysv_abi/sub_n.s
            src/apn/x64/sysv_abi/sub_one.s
            src/apn/x64/sysv_abi/neg.s
            src/apn/x64/sysv_abi/lshift_lt64.s
            src/apn/x64/sysv_abi/rshift_lt64.s
        )
    endif()

    set_source_files_properties(
        ${x64_asm_sources}
        PROPERTIES
            LANGUAGE ASM
    )

    set(isa_sources 
        
        "src/apn/x64/x64_hidden_funcs.h" 
        ${x64_c_sources} 
        ${x64_asm_sources}
    )
endif()

# -------------------------------------------------------------------------------
# HEADERS
# -------------------------------------------------------------------------------

set(hidden_headers
    src/apn/commons/hidden_mul.h
    src/apn/commons/hidden_sqr.h
    src/apn/commons/hidden_div.h
    src/apn/commons/hidden_helpers.h
    src/apn/commons/apn_thresholds.h
)

# -------------------------------------------------------------------------------
# FINAL SOURCE LIST
# -------------------------------------------------------------------------------

set(apac_sources
    ${apac_c_sources}
    ${isa_sources}
    ${hidden_headers}
)

# -------------------------------------------------------------------------------
# STRIP PATHS FROM __FILE__ (C ONLY)
# -------------------------------------------------------------------------------

foreach(src_file ${apac_c_sources} ${x64_c_sources})
    get_filename_component(file_name ${src_file} NAME)
    set_source_files_properties(
        ${src_file}
        PROPERTIES
            COMPILE_DEFINITIONS "__FILE__=\"${file_name}\""
    )
endforeach()

# -------------------------------------------------------------------------------
# LIBRARY TARGET
# -------------------------------------------------------------------------------

if (BUILD_SHARED_LIB)
    add_library(apac SHARED ${apac_sources})
    target_compile_definitions(apac PRIVATE BUILD_SHARED_LIB)
else()
    add_library(apac STATIC ${apac_sources})
endif()

# -------------------------------------------------------------------------------
# WARNING & FLAG HYGIENE (IMPORTANT FIXES)
# -------------------------------------------------------------------------------

# Silence intentional __FILE__ override (Clang only)
target_compile_options(apac PRIVATE
    -Wno-builtin-macro-redefined
)

# Apply optimization & NDEBUG ONLY to C, never ASM
target_compile_options(apac PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-O3>
)

target_compile_definitions(apac PRIVATE
    $<$<COMPILE_LANGUAGE:C>:NDEBUG>
    $<$<CONFIG:Release>:APAC_DISABLE_ASSERT>
)
