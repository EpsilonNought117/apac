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
    src/misc/setMemFuncs.c
    src/misc/getCPUSpec.c
    src/misc/apacInit.c
)

# Manually list Assembly source files if needed
set(ASM_SOURCES
    # src/asm/some_file.asm
)

# Default to static unless explicitly building shared
if(BUILD_SHARED_LIB)
    add_library(apac_shared SHARED ${C_SOURCES} ${ASM_SOURCES})
    message(STATUS "Building libapac as a shared library (DLL)")

    set_target_properties(apac_shared PROPERTIES OUTPUT_NAME "apac_shared")

    set_target_properties(apac_shared PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_INSTALL_PREFIX}/lib"  # .lib (import library)
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_INSTALL_PREFIX}/bin"  # .dll
    )

    target_include_directories(apac_shared INTERFACE "include" PRIVATE "src")

    install(TARGETS apac_shared
        CONFIGURATIONS Release
        LIBRARY DESTINATION lib    # .lib (import library)
        RUNTIME DESTINATION bin    # .dll
    )

else()
    add_library(apac_static STATIC ${C_SOURCES} ${ASM_SOURCES})
    message(STATUS "Building libapac as a static library")

    set_target_properties(apac_static PROPERTIES OUTPUT_NAME "apac_static")

    set_target_properties(apac_static PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_INSTALL_PREFIX}/lib"  # Static .lib
    )

    target_include_directories(apac_static INTERFACE "include" PRIVATE "src")

    install(TARGETS apac_static
        CONFIGURATIONS Release
        ARCHIVE DESTINATION lib    # .lib (static library)
    )
endif()