@echo off
setlocal

REM # detectare compiler
set CC=gcc

REM # directoare si fisiere
set SDL_PATH=%~dp0lib\windows
set BUILD_DIR=build
set SRC=src\main.c src\ui.c src\game.c src\list.c src\utils.c src\cJSON.c src\cJSON_Utils.c
set OBJ=%BUILD_DIR%\main.o %BUILD_DIR%\ui.o %BUILD_DIR%\game.o %BUILD_DIR%\list.o %BUILD_DIR%\utils.o %BUILD_DIR%\cJSON.o %BUILD_DIR%\cJSON_Utils.o

REM # flaguri compilare si link
set INCLUDE_FLAGS=-Isrc -Iinclude -I"%SDL_PATH%\include"
set LINK_FLAGS=-L"%SDL_PATH%\lib" -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image

REM # creare folder build
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

REM # curatare
echo [clean] stergere .o si .exe vechi
del /Q %BUILD_DIR%\*.o 2>nul
del /Q %BUILD_DIR%\hangman.exe 2>nul

REM # compilare
echo [build] compilare fisiere sursa
for %%f in (%SRC%) do (
  echo   compilare %%f...
  %CC% -c %%f -o %BUILD_DIR%\%%~nf.o %INCLUDE_FLAGS%
  if errorlevel 1 (
    echo [eroare] compilare esuata pentru %%f
    goto end
  )
)

REM # link
echo [link] creare executabil
%CC% -o %BUILD_DIR%\hangman.exe %OBJ% %LINK_FLAGS%
if errorlevel 1 (
  echo [eroare] linkare esuata
  goto end
)

REM # copiere DLL-uri
echo [copy] SDL2.dll, SDL2_ttf.dll, SDL2_image.dll in build
copy /Y "%SDL_PATH%\bin\SDL2.dll" %BUILD_DIR%\ >nul
copy /Y "%SDL_PATH%\bin\SDL2_ttf.dll" %BUILD_DIR%\ >nul
copy /Y "%SDL_PATH%\bin\SDL2_image.dll" %BUILD_DIR%\ >nul

REM # rulare
echo [run] executie hangman.exe
cd %BUILD_DIR%
hangman.exe
cd ..

:end
pause
endlocal
