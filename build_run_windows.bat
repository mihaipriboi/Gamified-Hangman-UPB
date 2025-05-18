@echo off
REM === Compilator, foldere si fisiere ===
set CC=gcc
set SRCDIR=src
set OBJDIR=build
set BINARY=%OBJDIR%\hangman.exe

REM === Include si lib ===
set CFLAGS=-Wall -Wextra -Werror -Iinclude
set LDFLAGS=-Llib\windows -lSDL2 -lSDL2_ttf

REM === Creeaza folderul de build daca nu exista ===
if not exist %OBJDIR% (
    mkdir %OBJDIR%
)

REM === Compilare ===
echo Compiling...
%CC% %CFLAGS% %SRCDIR%\*.c -o %BINARY% %LDFLAGS%
if errorlevel 1 (
    echo Eroare la compilare!
    exit /b 1
)

REM === Copierea bibliotecilor ===
copy /Y lib\windows\SDL2.dll %OBJDIR%\ >nul
copy /Y lib\windows\SDL2_ttf.dll %OBJDIR%\ >nul

REM === Rulare ===
echo Running...
cd %OBJDIR%
hangman.exe
cd ..
