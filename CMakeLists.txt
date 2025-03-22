cmake_minimum_required(VERSION 3.12)
project(libapac C)

# Ensure MSVC is 64-bit
if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    message(FATAL_ERROR "MSVC is required to build libapac.")
endif()
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Only 64-bit builds are supported.")
endif()

# Enable MASM support
enable_language(ASM_MASM)

# Options
option(BUILD_SHARED_LIB "Build libapac as a shared (DLL) library" OFF)

# Manually list common C source files
set(C_SOURCES
    # MISC C SOURCES
    src/misc/setMemFuncs.c
    src/misc/getCPUSpec/getCPUSpec.c
    src/misc/apacInitDefault.c

    # APN C SOURCES
    src/apn/common/apn_add_sub/apn_add_sub.c
    src/apn/common/apn_cpy/apn_cpy.c
)

# Manually list assembly source files
set(ASM_SOURCES
    
    src/apn/x64/_adc_apn_add_n.asm
    src/apn/x64/_sbb_apn_sub_n.asm
    
)

# Combine all source files
set(ALL_SOURCES ${C_SOURCES} ${ASM_SOURCES})

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose build type: Debug or Release" FORCE)
endif()

# Enforce specific optimization flags for MSVC
if(MSVC)
    add_compile_options(
        "$<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Release>>:/O2>"   # Optimize C files in Release mode
        "$<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Debug>>:/Od>"     # Disable optimizations in Debug mode
        "$<$<COMPILE_LANGUAGE:C>:/W4>"                            # Enable warnings
        "$<$<COMPILE_LANGUAGE:C>:/WX>"                            # Treat warnings as errors
    )
endif()


# Build either the shared OR static library (not both)
if(BUILD_SHARED_LIB)
    add_library(apac SHARED ${ALL_SOURCES})
    message(STATUS "Building shared library (DLL)")

    # Define LIBAPAC_IMPORT for shared library
    target_compile_definitions(apac PRIVATE LIBAPAC_DLL)

    set_target_properties(apac PROPERTIES 
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    install(TARGETS apac
        LIBRARY DESTINATION "lib"
        RUNTIME DESTINATION "bin"
        ARCHIVE DESTINATION "lib"
    )
else()
    add_library(apac STATIC ${ALL_SOURCES})
    message(STATUS "Building static library")

    set_target_properties(apac PROPERTIES 
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    )

    install(TARGETS apac ARCHIVE DESTINATION "lib")
endif()

message(STATUS "Installation directory: ${CMAKE_INSTALL_PREFIX}")
