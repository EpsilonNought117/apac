
#   O---------------------------------------------------------------------------O
#   |                                                                           |
#   |                          LIBAPAC CMAKE BUILD FILE                         | 
#   |                                                                           |
#   O---------------------------------------------------------------------------O


# --- CHECKS --------------------------------------------------------------------
    
    cmake_minimum_required(VERSION 3.12)
    project(libapac C)

    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(FATAL_ERROR "Only 64-bit builds are supported.")
    endif()

    # Find NASM assembler (required for all platforms)

    find_program(NASM_EXECUTABLE nasm)

    if(NOT NASM_EXECUTABLE)
        message(FATAL_ERROR "NASM assembler is required to build libapac!")
    else()
        message(STATUS "Found Netwide Assembler: ${NASM_EXECUTABLE}")
        enable_language(ASM_NASM)
    endif()

    # Platform-specific compiler requirements
    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        
        if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC")
            message(FATAL_ERROR "MSVC Toolchain is required to build libapac on Windows!")
        endif()
    
        set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f win64 -d _WIN64")

    elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        
        if (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
            message(STATUS "Found supported compiler: ${CMAKE_C_COMPILER_ID} C Compiler")
        else()
            message(FATAL_ERROR "GCC/Clang is required to build libapac on Linux!")
        endif()
        
        # Set NASM flags for Linux
        set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf64 -d __linux__")

    else()
        message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
    endif()

    set(CMAKE_C_STANDARD 11)

# --- BUILD OPTIONS -------------------------------------------------------------

    # Option to build a DLL instead of a static lib

    option(BUILD_SHARED_LIBS "Build libapac as a shared library" OFF)

# --- COMMON C SOURCES ----------------------------------------------------------

    set(apac_c_sources

        # MISCELLANEOUS

            src/misc/apacSetMemFuncs.c
            src/misc/apacGetCPUSpec.c
            src/misc/apacInit.c

        # APN PUBLIC API

            src/apn/commons/apn_add/apn_add.c
            src/apn/commons/apn_sub/apn_sub.c
            src/apn/commons/apn_negate/apn_negate.c
            src/apn/commons/apn_mul/apn_mul.c
            src/apn/commons/apn_set/apn_set.c
            src/apn/commons/apn_cpy/apn_cpy.c

        # APN HIDDEN FUNCS

            src/apn/commons/apn_mul/_basecase_mul.c
            src/apn/commons/apn_mul/_karatsuba_mul.c
    )

# --- x64 SPECIFIC C/ASM SOURCES ------------------------------------------------

    set(x64_specific

        # ASSEMBLY SOURCES

            # ADDITION AND RELATED OPS
            
                src/apn/x64/_add/_add_n.asm
                src/apn/x64/_add/_add_n_one.asm
                src/apn/x64/_add/_add_n_one_till_carry.asm

            # BASECASE MULTIPLICATION
    
                src/apn/x64/_mul_bc/_mul_bc_mulx_adx.asm
                src/apn/x64/_mul_bc/_mul_bc_mulx.asm
                src/apn/x64/_mul_bc/_mul_bc.asm
    
            # SUBTRACTION AND RELATED OPS
    
                src/apn/x64/_sub/_sub_n.asm
                src/apn/x64/_sub/_sub_n_one.asm
                src/apn/x64/_sub/_sub_n_one_till_borrow.asm

            # NEGATION (2's Complement)
            
                src/apn/x64/_neg/_neg.asm

        # C SOURCES

            # SET (TO A VALUE) OPS

                src/apn/x64/_set/_set_sse2.c
                src/apn/x64/_set/_set_avx.c
                src/apn/x64/_set/_set_avx512f.c

            # COPYING OPS

                src/apn/x64/_cpy/_cpy_sse2.c
                src/apn/x64/_cpy/_cpy_avx.c
                src/apn/x64/_cpy/_cpy_avx512f.c

        # UARCH PARAM FILES

            src/apn/x64/uarch_params/zen4_params.c
    )

# --- HIDDEN HEADERS ------------------------------------------------------------

    set(hidden_headers

         # COMMONS

            src/apn/commons/apn_mul/_hidden_mul.h
            src/apn/commons/apn_thresholds.h
    
         # x64 SPECIFIC

            src/apn/x64/x64_hidden_funcs.h
    )

# --- BUILDING DLL/STATIC -------------------------------------------------------

    # Combine all source files
    set(apac_sources ${apac_c_sources} ${x64_specific} ${hidden_headers})

    if(BUILD_SHARED_LIBS)
        add_library(apac SHARED ${apac_sources})
        target_compile_definitions(apac PRIVATE BUILD_SHARED_LIBS)
        set(apac_target apac)
    else()
        add_library(apac STATIC ${apac_sources})
        set(apac_target apac)
    endif()

    # Configuration-specific definitions
    target_compile_definitions(${apac_target} PRIVATE
        "$<$<CONFIG:Release>:APAC_DISABLE_ASSERT>"
    )