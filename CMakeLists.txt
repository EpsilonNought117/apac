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

set(apac
    src/misc/setMemFuncs.c
    src/misc/getCPUSpec.c
    src/misc/apacInitDefault.c

    src/apn/apn_add_sub/apn_add_sub.c
    src/apn/apn_cpy/apn_cpy.c
    src/apn/apn_negate/apn_negate.c
    src/apn/apn_mul/apn_mul.c
)

# Assembly source files based on microarchitecture
set(apn_assembly_dir src/apn/assembly)
foreach(uarch IN LISTS TARGET_X64_UARCH)
    file(GLOB asm_files "${apn_assembly_dir}/${uarch}/*.asm")
    list(APPEND apac ${asm_files})
endforeach()

# Build shared or static library based on BUILD_SHARED_LIBS
if(BUILD_SHARED_LIBS)
    add_library(apac_shared SHARED ${apac})
    target_compile_definitions(apac_shared PRIVATE BUILD_SHARED_LIBS)
else()
    add_library(apac_static STATIC ${apac})
endif()
