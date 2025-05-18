#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "types.h"

#define MAX_GRESITE 5

void joc_pe_nivel(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size, Nod* nivel, StareJoc* stare);