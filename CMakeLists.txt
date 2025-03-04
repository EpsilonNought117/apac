cmake_minimum_required(VERSION 3.8)

project(libapac C)

# Ensure MSVC is used
if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    message(FATAL_ERROR "MSVC is required to build libapac.")
endif()

# Enable MASM support
enable_language(ASM_MASM)

# Options
option(VERBOSE_OUTPUT "Enable verbose output during compilation" OFF)
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

# Public headers
set(PUBLIC_HEADERS include/apac.h)

# Library type
if(BUILD_SHARED_LIB)
    add_library(libapac SHARED ${C_SOURCES} ${ASM_SOURCES})
    message(STATUS "Building libapac as a shared library (DLL)")
else()
    add_library(libapac STATIC ${C_SOURCES} ${ASM_SOURCES})
    message(STATUS "Building libapac as a static library")
endif()

# Set output directories
set_target_properties(libapac PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib"  # Moves .lib out of build dir
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/bin"  # Moves .dll out of build dir
)

# Include directories
target_include_directories(libapac
    PUBLIC "include"  # Exposes only apac.h
    PRIVATE "src"      # Internal headers
)

# Install rules
install(TARGETS libapac
    LIBRARY DESTINATION lib    # .lib (import library)
    RUNTIME DESTINATION bin    # .dll (shared library)
)

install(FILES include/apac.h DESTINATION include)

# Verbose output
if(VERBOSE_OUTPUT)
    message(STATUS "Compiling files:")
    foreach(FILE ${C_SOURCES})
        message(STATUS "  ${FILE}")
    endforeach()
endif()
