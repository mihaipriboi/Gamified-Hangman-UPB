#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "types.h"

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

// Layout general
#define CENTER_X 0.5f

// Layout relativ pentru butoane
#define BTN_REL_Y_START 0.2f
#define BTN_REL_Y_STEP 0.1f
#define BTN_REL_W 0.25f
#define BTN_REL_H 0.08f

// Dimensiuni minime si maxime pentru butoane
#define BTN_MIN_W 150
#define BTN_MAX_W 300
#define BTN_MIN_H 40
#define BTN_MAX_H 80

// Setari pentru marimea fontului
#define FONT_SCALE_DIVISOR 32
#define FONT_MIN_SIZE 18
#define FONT_MAX_SIZE 36

// Fonturi
#define FONT_PATH "assets/fonts/DejaVuSans.ttf"
#define FONT_VARIANTS 6

extern TTF_Font *fonts[FONT_VARIANTS];

// Functii pentru incarcarea fonturilor
void loadFonts();
void unloadFonts();
TTF_Font* getFontForWindow(AbsoluteSize win_size);

// Functii UI
void afiseaza_meniu(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size, Buton butoane[], int mouse_x, int mouse_y);
void afiseaza_instructiuni(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size);
void afiseaza_documentatie_select(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size, Buton but1, Buton but2, int mouse_x, int mouse_y);
void afiseaza_documentatie_parte(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size, int parte, Buton back_btn, int mouse_x, int mouse_y, StareJoc* stare);

// Functii auxiliare
bool mouse_over(SDL_Rect rect, int mx, int my);
void set_render_color(SDL_Renderer* renderer, SDL_Color color);
void render_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color);
void render_text_relative(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size, const char* text, RelativePos rel, SDL_Color color);
void render_button(SDL_Renderer* renderer, TTF_Font* font, Buton buton, bool hover);
void render_simple_button(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect rect, const char* text, bool hover);

// Functie pentru calcularea pozitiei si dimensiunii scalate a unui element
SDL_Rect get_scaled_rect(AbsoluteSize window, RelativeRect rel);