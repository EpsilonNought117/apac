@echo off
setlocal enabledelayedexpansion

:: Configuration
set BUILD_DIR=build
set INSTALL_DIR=install
set TARGET_UARCH=alderlake
set BUILD_SHARED_LIB=OFF
set GENERATOR=Ninja

:: Clean previous build and install directories
echo Cleaning build and install directories...
if exist %BUILD_DIR% rmdir /s /q %BUILD_DIR%
if exist %INSTALL_DIR% rmdir /s /q %INSTALL_DIR%

:: Configure CMake
echo Configuring CMake...
cmake -G "%GENERATOR%" -S . -B %BUILD_DIR% -DCMAKE_BUILD_TYPE=Release -DTARGET_UARCH=%TARGET_UARCH% -DBUILD_SHARED_LIB=%BUILD_SHARED_LIB%

:: Build the project
echo Building the project...
cmake --build %BUILD_DIR%

:: Install the project
echo Installing the project...
cmake --install %BUILD_DIR%

echo Build and installation complete
endlocal
