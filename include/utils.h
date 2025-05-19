#pragma once

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "cJSON.h"

#include "utils.h"
#include "style.h"
#include "types.h"
#include "list.h"

bool is_word_guessed(const char* word, const char* display);
int citeste_documentatie_din_json(const char* filename, char titluri[][128], char texte[][1024], int max_elem);
void update_display_word(const char* word, const bool guessed[26], char* display);