@echo off
echo ============================================
echo   Ad Slot Allocator - DAA Project
echo   Compiling C program...
echo ============================================

gcc src\ad_scheduler.c -o ad_scheduler.exe -lm

if %ERRORLEVEL% NEQ 0 (
    echo ========================================================
    echo [ERROR] GCC compiler not found in PATH!
    echo.
    echo Please follow these steps:
    echo 1. Download: https://github.com/brechtsanders/winlibs_mingw/releases/download/15.2.0posix-14.0.0-ucrt-r7/winlibs-x86_64-posix-seh-gcc-15.2.0-mingw-w64ucrt-14.0.0-r7.zip
    echo 2. Extract the ZIP to C:\mingw64
    echo 3. Click Start -> type 'env' -> Edit environment variables
    echo 4. Edit 'Path' -> Add 'C:\mingw64\bin'
    echo 5. Restart your Terminal or Code Editor
    echo ========================================================
    pause
    exit /b 1
)

echo [OK] Compiled successfully: ad_scheduler.exe
echo.
echo Starting Node.js server...
echo Open http://localhost:3000 in your browser
echo Press Ctrl+C to stop
echo.
node server.js
pause
