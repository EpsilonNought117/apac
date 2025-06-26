
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                          LIBAPAC CMAKE BUILD FILE                         | 
#   |                                                                           |
#   O---------------------------------------------------------------------------O

# --- CHECKS --------------------------------------------------------------------
    
    cmake_minimum_required(VERSION 3.12)
    project(libapac C)
    
    # Check for 64-bit architecture
    
    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(FATAL_ERROR "Only 64-bit builds are supported.")
    endif()
    
    # Platform and compiler checks

    if(WIN32)

        if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC")
            message(FATAL_ERROR "MSVC Toolchain is required to build libapac on Windows!")
        endif()

        enable_language(ASM_MASM)
        message(STATUS "Building for Windows with MSVC and MASM")

    elseif(UNIX)

        if(NOT CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
            message(FATAL_ERROR "GCC or Clang compiler is required to build libapac on Unix!")
        endif()

        enable_language(ASM)
        message(STATUS "Building for Unix with ${CMAKE_C_COMPILER_ID}")

    else()

        message(FATAL_ERROR "Unsupported platform. Only Windows and Unix are supported.")
    
    endif()
    
    set(CMAKE_C_STANDARD 11)
    
# --- BUILD OPTIONS -------------------------------------------------------------

    # Option to build a DLL instead of a static lib

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

        # APN HIDDEN FUNCS

        "src/apn/commons/_basecase_mul.c"
        "src/apn/commons/_karatsuba_mul.c"

    )
    
# --- x64 SPECIFIC C SOURCES ----------------------------------------------------

    set(x64_c_sources

        # C SOURCES

        "src/apn/x64/_set.c"
        "src/apn/x64/_cpy.c"

        # UARCH PARAM FILES

        "src/apn/x64/cpu_params.c"
    )
    
# --- PLATFORM SPECIFIC ASSEMBLY SOURCES ---------------------------------------

    if(WIN32)

        set(x64_asm_sources

            # MS-ABI ASSEMBLY CODE (Windows + MSVC + MASM)

            "src/apn/x64/ms-abi/_add_n.asm"
            "src/apn/x64/ms-abi/_add_n_one.asm"
            "src/apn/x64/ms-abi/_add_n_one_till_carry.asm"    
            "src/apn/x64/ms-abi/_mul_bc.asm"
            "src/apn/x64/ms-abi/_sub_n.asm"
            "src/apn/x64/ms-abi/_sub_n_one.asm"
            "src/apn/x64/ms-abi/_sub_n_one_till_borrow.asm"
            "src/apn/x64/ms-abi/_neg.asm"
        )

    elseif(UNIX)

        set(x64_asm_sources

            # SYSV-ABI ASSEMBLY CODE (Unix + GCC/Clang)

            "src/apn/x64/sysv-abi/_add_n.s"
            "src/apn/x64/sysv-abi/_add_n_one.s"
            "src/apn/x64/sysv-abi/_add_n_one_till_carry.s"    
            "src/apn/x64/sysv-abi/_mul_bc.s"
            "src/apn/x64/sysv-abi/_sub_n.s"
            "src/apn/x64/sysv-abi/_sub_n_one.s"
            "src/apn/x64/sysv-abi/_sub_n_one_till_borrow.s"
            "src/apn/x64/sysv-abi/_neg.s"
        )

    endif()
    
# --- HIDDEN HEADERS ------------------------------------------------------------

    set(hidden_headers

         # COMMONS

            "src/apn/commons/_hidden_mul.h"
            "src/apn/commons/apn_thresholds.h"
    
         # x64 SPECIFIC

            "src/apn/x64/x64_hidden_funcs.h"
    )
    
# --- BUILDING DLL/STATIC -------------------------------------------------------

    # Combine all source files

    set(apac_sources ${apac_c_sources} ${x64_c_sources} ${x64_asm_sources} ${hidden_headers})

    # Build Shared/Static Library
    
    if(BUILD_SHARED_LIB)
        add_library(apac SHARED ${apac_sources})
        target_compile_definitions(apac PRIVATE BUILD_SHARED_LIB)
        set(apac_target apac)
    else()
        add_library(apac STATIC ${apac_sources})
        set(apac_target apac)
    endif()
    
    # Configuration-specific definitions

    target_compile_definitions(${apac_target} PRIVATE
        "$<$<CONFIG:Release>:APAC_DISABLE_ASSERT>"
    )