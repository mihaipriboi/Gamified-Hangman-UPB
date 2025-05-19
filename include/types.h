#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef enum {
  MENIU,
  PARTEA_I,
  PARTEA_II,
  INSTRUCTIUNI,
  IESIRE,
  DOCUMENTATIE_SELECT,
  DOCUMENTATIE_PARTE1,
  DOCUMENTATIE_PARTE2
} StareJoc;

typedef struct Nod {
  char hint[256];
  char answer[64];
  struct Nod* prev;
  struct Nod* next;
} Nod;

typedef struct {
  SDL_Rect rect;
  const char* text;
} Buton;

#define NULL_RECT {0, 0, 0, 0}

typedef struct {
  float x, y, w, h; // valori relative (0.0 - 1.0)
} RelativeRect;

typedef struct {
  float x, y; // valori relative (0.0 - 1.0)
} RelativePos;

typedef struct {
  int w, h; // dimensiuni absolute
} AbsoluteSize;