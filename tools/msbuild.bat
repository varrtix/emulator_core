@echo off
setlocal enabledelayedexpansion

REM Get the directory where the script is located
set "SCRIPT_DIR=%~dp0"

REM Set the root directory
set "ROOT_DIR=%SCRIPT_DIR%.."

REM Navigate to the root directory
cd /d "%ROOT_DIR%"

REM Remove the build directory if it exists
if exist "build" (
    echo Removing build directory ...
    rd /s /q "build"
)

REM Check if cmake is set in the environment variables
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    set "CMAKE_EXE=C:\Program Files\CMake\bin\cmake.exe"
    echo Using default cmake path: !CMAKE_EXE!
) else (
    set "CMAKE_EXE=cmake"
    echo Using cmake from PATH
)

REM Run cmake with all script arguments and build using Ninja
echo Running cmake ...
"!CMAKE_EXE!" -G Ninja -S . -B build -DCMAKE_CXX_COMPILER=cl -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX="%ROOT_DIR%\dist" -DENABLE_EMU=ON %*
echo Building ...
"!CMAKE_EXE!" --build build --parallel 32
echo Installing ...
"!CMAKE_EXE!" --install build

endlocal
