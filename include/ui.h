#pragma once

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "ui.h"
#include "style.h"
#include "types.h"
#include "list.h"
#include "utils.h"

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

// Layout general
#define CENTER_X 0.5f

// Layout relativ pentru butoane
#define BTN_REL_Y_START 0.3f
#define BTN_REL_Y_STEP 0.12f
#define BTN_REL_W 0.25f
#define BTN_REL_H 0.08f

// Dimensiuni minime si maxime pentru butoane
#define BTN_MIN_W 150
#define BTN_MAX_W 400
#define BTN_MIN_H 40
#define BTN_MAX_H 80

// Dimensiuni minime si maxime pentru imagini
#define IMG_MIN_W 100
#define IMG_MAX_W 600
#define IMG_MIN_H 100
#define IMG_MAX_H 600

// Fonturi
#define FONT_PATH "assets/fonts/DejaVuSans.ttf"
#define FONT_VARIANTS 10
#define FONT_TITLE_VARIANTS 2

extern TTF_Font *fonts[FONT_VARIANTS + FONT_TITLE_VARIANTS];

// Functii pentru incarcarea fonturilor
void loadFonts();
void unloadFonts();
TTF_Font* getFontForWindow(AbsoluteSize win_size);
TTF_Font* getFontForWindowTitle(AbsoluteSize win_size);

// Functii UI
void afiseaza_meniu(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size, Buton butoane[], int mouse_x, int mouse_y);
void afiseaza_instructiuni(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size);
void afiseaza_documentatie_select(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size, Buton* but1, Buton* but2, int mouse_x, int mouse_y);
void afiseaza_documentatie_parte(SDL_Renderer* renderer, TTF_Font* font, TTF_Font* font_title, AbsoluteSize win_size, int parte, Buton back_btn, Buton before_btn, Buton next_btn, int mouse_x, int mouse_y, StareJoc* stare);

// Functii auxiliare
bool mouse_over(SDL_Rect rect, int mx, int my);
void set_render_color(SDL_Renderer* renderer, SDL_Color color);
void render_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color);
void render_text_relative(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size, const char* text, RelativePos rel, SDL_Color color);
void render_text_button(SDL_Renderer* renderer, TTF_Font* font, Buton buton, const char* text, SDL_Color color);
void render_centered_wrapped_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Rect rect, SDL_Color color);
void render_button(SDL_Renderer* renderer, TTF_Font* font, Buton buton, bool hover);
void render_simple_button(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect rect, const char* text, bool hover);

// Functie pentru calcularea pozitiei si dimensiunii scalate a unui element
SDL_Rect get_scaled_rect(AbsoluteSize window, RelativeRect rel);
SDL_Rect get_scaled_img(AbsoluteSize window, RelativeRect rel);