cmake_minimum_required(VERSION 3.12)
project(libapac C)

if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    message(FATAL_ERROR "MSVC is required to build libapac.")
endif()
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Only 64-bit builds are supported.")
endif()

enable_language(ASM_MASM)

# Define the target architectures
set(TARGET_X64_UARCH alderlake generic)

# Base source files
set(apac
    src/misc/setMemFuncs.c
    src/misc/getCPUSpec.c
    src/misc/apacInitDefault.c

    src/apn/apn_add_sub/apn_add_sub.c
    src/apn/apn_cpy/apn_cpy.c
    src/apn/apn_negate/apn_negate.c
    src/apn/apn_mul/apn_mul.c
)

# Explicit list of assembly files for each UARCH
set(alderlake
    src/apn/assembly/alderlake/_apn_add_n.asm
    src/apn/assembly/alderlake/_apn_sub_n.asm
    src/apn/assembly/alderlake/_apn_mul_basecase.asm
)

set(generic

    
)

# Add corresponding assembly files based on selected microarchitecture
foreach(uarch IN LISTS TARGET_X64_UARCH)
    if(uarch STREQUAL "alderlake")
        list(APPEND apac ${alderlake})
    elseif(uarch STREQUAL "generic")
        list(APPEND apac ${generic})
    endif()
endforeach()

# Build shared or static library based on BUILD_SHARED_LIBS
if(BUILD_SHARED_LIBS)
    add_library(apac_shared SHARED ${apac})
    target_compile_definitions(apac_shared PRIVATE BUILD_SHARED_LIBS)
else()
    add_library(apac_static STATIC ${apac})
endif()
