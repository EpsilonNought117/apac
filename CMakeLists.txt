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

# Define supported microarchitectures
set(SUPPORTED_UARCHES "alderlake" "generic")

# Set the default microarchitecture if not specified
if(NOT DEFINED TARGET_UARCH)
    set(TARGET_UARCH "generic" CACHE STRING "Target microarchitecture")
    set_property(CACHE TARGET_UARCH PROPERTY STRINGS ${SUPPORTED_UARCHES})
endif()

# Validate the selected microarchitecture
list(FIND SUPPORTED_UARCHES ${TARGET_UARCH} UARCH_INDEX)
if(UARCH_INDEX EQUAL -1)
    message(FATAL_ERROR "Unsupported microarchitecture: ${TARGET_UARCH}. Supported options are: ${SUPPORTED_UARCHES}")
endif()

message(STATUS "Building for microarchitecture: ${TARGET_UARCH}")

# Manually list common C source files
set(C_SOURCES
    # MISC C SOURCES
    src/misc/setMemFuncs.c
    src/misc/getCPUSpec.c
    src/misc/apacInitDefault.c

    # APN C SOURCES
    src/apn/common/apn_add_sub/apn_add_sub.c
    src/apn/common/apn_cpy/apn_cpy.c
)

# Set up paths
set(GENERIC_ASM_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/apn/x64/generic")
set(UARCH_ASM_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/apn/x64/${TARGET_UARCH}")

# Find all generic assembly files
file(GLOB GENERIC_ASM_FILES LIST_DIRECTORIES false "${GENERIC_ASM_PATH}/*.asm")

# Initialize the final assembly files list with generic files
set(FINAL_ASM_FILES ${GENERIC_ASM_FILES})

# If not generic, replace with specialized files
if(NOT TARGET_UARCH STREQUAL "generic")
    if(IS_DIRECTORY "${UARCH_ASM_PATH}")
        file(GLOB UARCH_ASM_FILES LIST_DIRECTORIES false "${UARCH_ASM_PATH}/*.asm")
        foreach(SPECIALIZED_FILE ${UARCH_ASM_FILES})
            get_filename_component(FILE_NAME ${SPECIALIZED_FILE} NAME)
            set(GENERIC_MATCH "")
            foreach(GENERIC_FILE ${GENERIC_ASM_FILES})
                get_filename_component(GENERIC_NAME ${GENERIC_FILE} NAME)
                if(GENERIC_NAME STREQUAL FILE_NAME)
                    set(GENERIC_MATCH ${GENERIC_FILE})
                    break()
                endif()
            endforeach()
            if(GENERIC_MATCH)
                list(REMOVE_ITEM FINAL_ASM_FILES ${GENERIC_MATCH})
            endif()
            list(APPEND FINAL_ASM_FILES ${SPECIALIZED_FILE})
        endforeach()
    else()
        message(WARNING "Microarchitecture-specific directory not found for ${TARGET_UARCH}. Using generic implementations.")
    endif()
endif()

set(ALL_SOURCES ${C_SOURCES} ${FINAL_ASM_FILES})

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose build type: Debug or Release" FORCE)
endif()

# Enforce specific optimization flags for MSVC
if(MSVC)
    add_compile_options(
        "$<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Release>>:/O2>"   # Optimize C files in Release mode
        "$<$<AND:$<COMPILE_LANGUAGE:C>,$<CONFIG:Debug>>:/Od>"        # Disable optimizations for C files in Debug mode
        "$<$<COMPILE_LANGUAGE:C>:/W4>"                               # Enable warnings for C files
        "$<$<COMPILE_LANGUAGE:C>:/WX>"                               # Treat warnings as errors for C files
    )
endif()

# Default install location (inside project)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install" CACHE PATH "Install location" FORCE)
endif()

# Build either the shared OR static library (not both)
if(BUILD_SHARED_LIB)
    add_library(apac SHARED ${ALL_SOURCES})
    message(STATUS "Building shared library (DLL)")

    # Define LIBAPAC_IMPORT for shared library
    target_compile_definitions(apac PRIVATE LIBAPAC_DLL)

    # Ensure import library (.lib) is created
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

# Include headers
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION "include" FILES_MATCHING PATTERN "*.h")

# Ensure the old install directory is removed before installing
add_custom_target(prepare_install DEPENDS remove_old_install)

message(STATUS "Installation directory: ${CMAKE_INSTALL_PREFIX}")
