@echo off
REM === compilator, foldere și fisiere ===
set CC=gcc
set SRC=main.c
set OBJDIR=build
set BINARY=%OBJDIR%\hangman.exe

REM === include si lib ===
set CFLAGS=-Wall -Wextra -Werror -Iinclude
set LDFLAGS=-Llib\windows -lSDL2 -lSDL2_ttf

REM === folder pentru build ===
if not exist %OBJDIR% (
    mkdir %OBJDIR%
)

REM === compilare ===
echo Compiling...
%CC% %CFLAGS% %SRC% -o %BINARY% %LDFLAGS%
if errorlevel 1 (
    echo Eroare la compilare!
    exit /b 1
)

REM === copierea bibliotecilor ===
copy /Y lib\windows\SDL2.dll %OBJDIR%\ >nul
copy /Y lib\windows\SDL2_ttf.dll %OBJDIR%\ >nul

REM === rularea ===
echo Running...
cd %OBJDIR%
hangman.exe
cd ..
