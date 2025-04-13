cmake_minimum_required(VERSION 3.12)
project(libapac C)

if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    message(FATAL_ERROR "MSVC is required to build libapac.")
endif()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Only 64-bit builds are supported.")
endif()

enable_language(ASM_MASM)

# Option to build shared or static
option(BUILD_SHARED_LIBS "Build libapac as a shared library" OFF)

# Option to build unit tests
option(BUILD_UNIT_TESTS "Build the unit test executable" OFF)

# Base source files
set(apac_c_sources

    # MISCELLANEOUS HEADERS
    src/misc/setMemFuncs.c
    src/misc/getCPUSpec.c
    src/misc/apacInitDefault.c

)

# x64-specific assembly and source files
set(x64_specific

    # PUBLIC API
    src/apn/x64/apn_add_sub/apn_add_sub.c
    src/apn/x64/apn_cpy/apn_cpy.c
    src/apn/x64/apn_negate/apn_negate.c
    src/apn/x64/apn_mul/apn_mul.c

    # HIDDEN
    src/apn/x64/apn_mul/_basecase_mul.c

    # ASSEMBLY
    src/apn/x64/assembly/_adc_add_n.asm
    src/apn/x64/assembly/_adc_add_one.asm
    src/apn/x64/assembly/_adc_till_carry.asm
    src/apn/x64/assembly/_adc_mulx_mul_basecase.asm
    src/apn/x64/assembly/_sbb_sub_n.asm
    src/apn/x64/assembly/_sbb_sub_one.asm
    src/apn/x64/assembly/_sbb_till_borrow.asm
)

# Combine all source files
set(apac ${apac_c_sources} ${x64_specific})

# Build shared or static libapac
if(BUILD_SHARED_LIBS)
    add_library(apac_shared SHARED ${apac})
    target_compile_definitions(apac_shared PRIVATE BUILD_SHARED_LIBS)
else()
    add_library(apac_static STATIC ${apac})
endif()

# Conditionally build unit test executable
if(BUILD_UNIT_TESTS)

endif()
