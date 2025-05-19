#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "game.h"
#include "style.h"
#include "ui.h"
#include "types.h"
#include "utils.h"
#include "list.h"

#define MAX_GRESITE 5

void joc_pe_nivel(SDL_Renderer* renderer, TTF_Font* font, TTF_Font* font_title, AbsoluteSize win_size, Nod* nivel, StareJoc* stare);