cmake_minimum_required(VERSION 3.8)

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

# Manually list C source files
set(C_SOURCES
    # miscellaneous C Sources
    src/misc/setMemFuncs.c
    src/misc/getCPUSpec.c
    src/misc/apacInitDefault.c

    # APN C Sources

    

    # APZ C Sources
)

# Manually list Assembly source files if needed
set(ASM_SOURCES
    # Add MASM files here if necessary
)

# Set install directories
set(INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")

# Default to static unless explicitly building shared
if(BUILD_SHARED_LIB)
    add_library(apac_shared SHARED ${C_SOURCES} ${ASM_SOURCES})
    message(STATUS "Building libapac as a shared library (DLL)")

    set_target_properties(apac_shared PROPERTIES OUTPUT_NAME "libapac")

    set_target_properties(apac_shared PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${INSTALL_DIR}/shared"  # .lib (import library)
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${INSTALL_DIR}/shared"  # .dll
    )

    target_include_directories(apac_shared INTERFACE "include" PRIVATE "src")

    install(TARGETS apac_shared
        CONFIGURATIONS Release
        LIBRARY DESTINATION release/shared  # .lib (import library)
        RUNTIME DESTINATION release/shared  # .dll
    )

else()
    add_library(apac_static STATIC ${C_SOURCES} ${ASM_SOURCES})
    message(STATUS "Building libapac as a static library")

    set_target_properties(apac_static PROPERTIES OUTPUT_NAME "libapac")

    set_target_properties(apac_static PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${INSTALL_DIR}/static"  # Static .lib
    )

    target_include_directories(apac_static INTERFACE "include" PRIVATE "src")

    install(TARGETS apac_static
        CONFIGURATIONS Release
        ARCHIVE DESTINATION release/static  # .lib (static library)
    )
endif()
