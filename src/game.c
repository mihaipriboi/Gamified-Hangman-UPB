#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "game.h"
#include "style.h"
#include "ui.h"
#include "types.h"
#include "utils.h"
#include "list.h"

#ifdef _WIN32
  #define ASSETS_PATH_BASE "../assets/images/"
#else
  #define ASSETS_PATH_BASE "assets/images/"
#endif

void joc_pe_nivel(SDL_Renderer* renderer, TTF_Font* font, TTF_Font* font_title, AbsoluteSize win_size, Nod* nivel, StareJoc* stare) {
  bool guessed[26] = {false};
  int gresite = 0;
  char display[128];

  SDL_StartTextInput();
  update_display_word(nivel->answer, guessed, display);
  SDL_Event event;
  bool in_nivel = true;
  bool corect = false;
  bool pierdut = false;

  while (in_nivel) {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    SDL_GetRendererOutputSize(renderer, &win_size.w, &win_size.h);
    font = getFontForWindow(win_size);

    Buton next_btn = {get_scaled_rect(win_size, (RelativeRect){CENTER_X, 0.9f, 0.2f, 0.07f}), "Nivelul urmator"};
    Buton retry_btn = {get_scaled_rect(win_size, (RelativeRect){0.15f, 0.9f, 0.2f, 0.07f}), "Reincearca"};
    Buton menu_btn  = {get_scaled_rect(win_size, (RelativeRect){0.85f, 0.9f, 0.2f, 0.07f}), "Inapoi la meniu"};

    // Verificare dimensiuni fereastra
    SDL_GetRendererOutputSize(renderer, &win_size.w, &win_size.h);

    // Font dinamic
    font = getFontForWindow(win_size);
    font_title = getFontForWindowTitle(win_size);

    while (SDL_PollEvent(&event)) {
      // Verificare dimensiuni fereastra
      SDL_GetRendererOutputSize(renderer, &win_size.w, &win_size.h);

      // Font dinamic
      font = getFontForWindow(win_size);
      font_title = getFontForWindowTitle(win_size);

      if (event.type == SDL_QUIT) {
        *stare = IESIRE;
        return;
      }
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        *stare = MENIU;
        return;
      }
      if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN) && corect) {
        in_nivel = false;
      }
      if (!corect && !pierdut && event.type == SDL_TEXTINPUT) {
        char litera = tolower(event.text.text[0]);
        if (isalpha(litera)) {
          int index = litera - 'a';
          if (!guessed[index]) {
            guessed[index] = true;
            if (!strchr(nivel->answer, litera)) {
              gresite++;
            }
            update_display_word(nivel->answer, guessed, display);
          }
        }
      }
      if (corect && event.type == SDL_MOUSEBUTTONDOWN) {
        if (mouse_over(next_btn.rect, mouse_x, mouse_y)) {
          in_nivel = false;
        }
      }
      if (pierdut && event.type == SDL_MOUSEBUTTONDOWN) {
        if (mouse_over(retry_btn.rect, mouse_x, mouse_y)) {
          gresite = 0;
          memset(guessed, 0, sizeof(guessed));
          update_display_word(nivel->answer, guessed, display);
          pierdut = false;
        }
        if (mouse_over(menu_btn.rect, mouse_x, mouse_y)) {
          *stare = MENIU;
          return;
        }
      }
    }

    set_render_color(renderer, COLOR_BACKGROUND);
    SDL_RenderClear(renderer);

    render_text_relative(renderer, font, win_size, "Hint:", (RelativePos){CENTER_X, 0.05f}, COLOR_TEXT);
    render_text_relative(renderer, font, win_size, nivel->hint, (RelativePos){CENTER_X, 0.1f}, COLOR_TEXT);

    char path[256];
    if (gresite < MAX_GRESITE) {
        snprintf(path, sizeof(path), "%shangman_%d.bmp", ASSETS_PATH_BASE, gresite);
    } else {
        snprintf(path, sizeof(path), "%sgame_over.bmp", ASSETS_PATH_BASE);
    }

    SDL_Surface* image = SDL_LoadBMP(path);
    if (image) {
      SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
      SDL_FreeSurface(image);
      SDL_Rect imgRect = get_scaled_img(win_size, (RelativeRect){CENTER_X, 0.2f, 0.3f, 0.3f});
      SDL_RenderCopy(renderer, texture, NULL, &imgRect);
      SDL_DestroyTexture(texture);
    }

    render_text_relative(renderer, font_title, win_size, display, (RelativePos){CENTER_X, 0.65f}, COLOR_TEXT);

    if (is_word_guessed(nivel->answer, display)) {
      corect = true;
      render_text_relative(renderer, font, win_size, "Corect!", (RelativePos){CENTER_X, 0.8f}, COLOR_TEXT);
      bool hover = mouse_over(next_btn.rect, mouse_x, mouse_y);
      render_button(renderer, font, next_btn, hover);
    } else if (gresite >= MAX_GRESITE) {
      pierdut = true;
      render_text_relative(renderer, font, win_size, "Ai pierdut nivelul.", (RelativePos){CENTER_X, 0.8f}, COLOR_TEXT);
      bool hover1 = mouse_over(retry_btn.rect, mouse_x, mouse_y);
      bool hover2 = mouse_over(menu_btn.rect, mouse_x, mouse_y);
      render_button(renderer, font, retry_btn, hover1);
      render_button(renderer, font, menu_btn, hover2);
    }

    SDL_RenderPresent(renderer);
  }

  SDL_StopTextInput();
}