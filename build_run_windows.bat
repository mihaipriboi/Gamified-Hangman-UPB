@echo off
setlocal enabledelayedexpansion

REM === compilator, directoare si fisiere ===
set CC=gcc
set SRCDIR=src
set OBJDIR=build
set BINARY=%OBJDIR%\hangman.exe

REM === flaguri pentru compilare si linkare (MinGW) ===
set CFLAGS=-Wall -Wextra -Werror -Iinclude -D__USE_MINGW_ANSI_STDIO=1
set LDFLAGS=-Llib\windows\mingw -lSDL2 -lSDL2_ttf -lm ^
  -lwinmm -lole32 -loleaut32 -luuid -lgdi32 -limm32 -lversion ^
  -lsetupapi -lcfgmgr32 -lrpcrt4

REM === creeaza directorul de build daca nu exista ===
if not exist %OBJDIR% (
    mkdir %OBJDIR%
)

REM === adauga fisierele sursa ===
set FILES=
for %%f in (%SRCDIR%\*.c) do (
    set FILES=!FILES! %%f
)

REM === compilare ===
echo Compilare in curs...
%CC% %CFLAGS% !FILES! -o %BINARY% %LDFLAGS%
if errorlevel 1 (
    echo Eroare la compilare!
    pause
    exit /b 1
)

if not exist %BINARY% (
    echo Executabilul nu a fost creat!
    pause
    exit /b 1
)

REM === copiere fisiere DLL necesare ===
copy /Y lib\windows\mingw\SDL2.dll %OBJDIR%\ >nul
copy /Y lib\windows\mingw\SDL2_ttf.dll %OBJDIR%\ >nul

REM === rulare executabil ===
echo Rulare...
cd %OBJDIR%
hangman.exe
echo.
pause
cd ..
