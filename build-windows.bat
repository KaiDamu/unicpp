@echo off

REM ===============================
REM Create build folder structure
REM ===============================
mkdir build
mkdir build\windows
cd build\windows

REM ===============================
REM Generate Visual Studio solution
REM ===============================
cmake -G "Visual Studio 17 2022" ..\..

REM ===============================
REM Build both Release and Debug
REM ===============================
echo.
echo ===== Building Release =====
cmake --build . --config Release

echo.
echo ===== Building Debug =====
cmake --build . --config Debug

echo.
echo Build complete!
pause
