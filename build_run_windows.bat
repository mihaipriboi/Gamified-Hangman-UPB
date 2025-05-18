@echo off
REM === Compiler, folders and files ===
set CC=gcc
set SRCDIR=src
set OBJDIR=build
set BINARY=%OBJDIR%\hangman.exe

REM === Includes and libs ===
set CFLAGS=-Wall -Wextra -Werror -Iinclude
set LDFLAGS=-Llib\windows -lSDL2 -lSDL2_ttf -lmingw32 -lm

REM === Create build dir if not exists ===
if not exist %OBJDIR% (
    mkdir %OBJDIR%
)

REM === Compile ===
echo Compiling...
%CC% %CFLAGS% %SRCDIR%\*.c -o %BINARY% %LDFLAGS%
if errorlevel 1 (
    echo Eroare la compilare!
    pause
    exit /b 1
)

if not exist %BINARY% (
    echo Compilation failed: binary not found.
    pause
    exit /b 1
)

REM === Copy DLLs ===
copy /Y lib\windows\SDL2.dll %OBJDIR%\ >nul
copy /Y lib\windows\SDL2_ttf.dll %OBJDIR%\ >nul

REM === Run ===
echo Running...
cd %OBJDIR%
hangman.exe
echo.
pause
cd ..
