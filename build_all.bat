@echo off
setlocal
echo.
echo ========================================
echo   C++ GAME STUDIO - BUILD SYSTEM
echo ========================================
echo.

:: Check for g++
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] g++ not found. Please install MinGW and add to PATH.
    pause
    exit /b 1
)

:: Build Tic-Tac-Toe Classic
echo [1/2] Building Tic-Tac-Toe Classic...
cd TicTacToe
g++ -static-libgcc -static-libstdc++ main.cpp -o tictactoe.exe
if %errorlevel% equ 0 (
    echo [SUCCESS] tictactoe.exe created.
) else (
    echo [FAILED] Failed to build Tic-Tac-Toe Classic.
)
cd ..

echo.

:: Build Tic-Tac-Toe Pro
echo [2/2] Building Tic-Tac-Toe Pro (AI Edition)...
cd SmartTicTacToe
g++ -static-libgcc -static-libstdc++ main.cpp -o smart_tictactoe.exe
if %errorlevel% equ 0 (
    echo [SUCCESS] smart_tictactoe.exe created.
) else (
    echo [FAILED] Failed to build Tic-Tac-Toe Pro.
)
cd ..

echo.
echo ========================================
echo   BUILD COMPLETE
echo ========================================
echo.
pause
