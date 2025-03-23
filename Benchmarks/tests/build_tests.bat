@echo off
setlocal

:: Ask the user for the full test file name
set /p TEST_FILE="Enter the test file to compile: "

:: Check if the file exists
if not exist "%TEST_FILE%" (
    echo Error: File "%TEST_FILE%" not found!
    exit /b 1
)

:: Extract the filename without extension
for %%F in ("%TEST_FILE%") do set EXE_NAME=%%~nF

:: Compiler and linker settings
set INCLUDE_DIRS=/I ../../include /I ../../install
set LIB_DIR=../../install/lib/apac.lib
set CFLAGS=/MD /O2
set LDFLAGS=/link

:: Compile and link the specified test file
cl %CFLAGS% "%TEST_FILE%" %INCLUDE_DIRS% %LDFLAGS% %LIB_DIR% /Fe:"%EXE_NAME%.exe"

endlocal
