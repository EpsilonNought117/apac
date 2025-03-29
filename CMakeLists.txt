cmake_minimum_required(VERSION 3.12)
project(libapac C)

if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    message(FATAL_ERROR "MSVC is required to build libapac.")
endif()
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Only 64-bit builds are supported.")
endif()

enable_language(ASM_MASM)

set(apac
    src/benchmark_tools/getCPUTime_Freq.c
    src/benchmark_tools/pinToCoreZero.c
    src/benchmark_tools/toggle_tb.c

    src/misc/setMemFuncs.c
    src/misc/getCPUSpec/getCPUSpec.c
    src/misc/apacInitDefault.c

    src/apn/x64/apn_add_sub/apn_add_sub.c
    src/apn/x64/apn_cpy/apn_cpy.c
    src/apn/x64/apn_negate/apn_negate.c

    src/apn/x64/assembly/_adc_apn_add_n.asm
    src/apn/x64/assembly/_sbb_apn_sub_n.asm
)

if(BUILD_SHARED_LIBS)
    add_library(apac_shared SHARED ${apac})
    target_compile_definitions(apac_shared PRIVATE BUILD_SHARED_LIBS)
else()
    add_library(apac_static STATIC ${apac})
endif()
