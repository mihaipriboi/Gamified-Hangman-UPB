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

TTF_Font *fonts[FONT_VARIANTS];

const int font_sizes[FONT_VARIANTS] = {12, 16, 20, 24, 28, 32};

void loadFonts() {
    for(int i = 0; i < FONT_VARIANTS; i++) {
        fonts[i] = TTF_OpenFont("assets/font.ttf", font_sizes[i]);
        if(!fonts[i]) {
            printf("Eroare la incarcarea fontului %d: %s\n", font_sizes[i], TTF_GetError());
            exit(1);
        }
    }
}

void unloadFonts() {
    for(int i = 0; i < FONT_VARIANTS; i++) {
        if(fonts[i]) {
            TTF_CloseFont(fonts[i]);
        }
    }
}

TTF_Font* getFontForWindow(AbsoluteSize win_size) {
    float base_w = 1280.0;
    float base_h = 720.0;
    float scale_x = win_size.w / base_w;
    float scale_y = win_size.h / base_h;
    float scale = (scale_x + scale_y) / 2.0;

    int index = (int)(scale * FONT_VARIANTS / 2.0);
    if(index < 0) index = 0;
    if(index >= FONT_VARIANTS) index = FONT_VARIANTS - 1;

    return fonts[index];
  }

void set_render_color(SDL_Renderer* renderer, SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void render_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
  SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect dst = {x, y, surface->w, surface->h};
  SDL_FreeSurface(surface);
  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

void render_text_relative(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size, const char* text, RelativePos rel, SDL_Color color) {
  SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int x = rel.x * win_size.w;
  int y = rel.y * win_size.h;
  if (rel.x == CENTER_X)
    x = (win_size.w - surface->w) / 2;

  SDL_Rect dst = {x, y, surface->w, surface->h};
  SDL_FreeSurface(surface);
  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

void render_button(SDL_Renderer* renderer, TTF_Font* font, Buton buton, bool hover) {
  SDL_Color color_background = hover ? COLOR_BUTTON_HOVER : COLOR_BUTTON_NORMAL;
  set_render_color(renderer, color_background);
  SDL_Rect r = buton.rect;
  SDL_RenderFillRect(renderer, &r);

  int text_x = buton.rect.x + 20;
  int text_y = buton.rect.y + 10;
  render_text(renderer, font, buton.text, text_x, text_y, COLOR_WHITE);
}

void render_simple_button(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect rect, const char* text, bool hover) {
  SDL_Color color_background = hover ? COLOR_BUTTON_SIMPLE_HOVER : COLOR_BUTTON_SIMPLE_NORMAL;
  set_render_color(renderer, color_background);
  SDL_RenderFillRect(renderer, &rect);

  int text_x = rect.x + 20;
  int text_y = rect.y + 10;
  render_text(renderer, font, text, text_x, text_y, COLOR_WHITE);
}

bool mouse_over(SDL_Rect rect, int mx, int my) {
  return mx >= rect.x && mx <= rect.x + rect.w && my >= rect.y && my <= rect.y + rect.h;
}

void afiseaza_meniu(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size, Buton butoane[], int mouse_x, int mouse_y) {
  set_render_color(renderer, COLOR_BACKGROUND);
  SDL_RenderClear(renderer);

  // text centrat relativ
  render_text_relative(renderer, font, win_size, "=== HANGMAN EDU ===", (RelativePos){CENTER_X, 0.1f}, COLOR_TEXT);

  for (int i = 0; i < 5; i++) {
      RelativeRect rel = {
          .x = CENTER_X,
          .y = BTN_REL_Y_START + i * BTN_REL_Y_STEP,
          .w = BTN_REL_W,
          .h = BTN_REL_H
      };
      butoane[i].rect = get_scaled_rect(win_size, rel);
      bool hover = mouse_over(butoane[i].rect, mouse_x, mouse_y);
      render_button(renderer, font, butoane[i], hover);
  }

  SDL_RenderPresent(renderer);
}

void afiseaza_instructiuni(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size) {
  set_render_color(renderer, COLOR_BACKGROUND);
  SDL_RenderClear(renderer);

  int line_height = win_size.h / 20;
  int x = win_size.w * 0.1f;
  int y = line_height * 3;

  render_text(renderer, font, "Scopul jocului este sa ghicesti...", x, y, COLOR_TEXT);
  render_text(renderer, font, "Ai 5 incercari gresite pentru fiecare nivel.", x, y + line_height, COLOR_TEXT);
  render_text(renderer, font, "Daca gresesti, ramai pe acelasi nivel.", x, y + 2 * line_height, COLOR_TEXT);
  render_text(renderer, font, "Daca te intorci la meniu, progresul se pierde.", x, y + 3 * line_height, COLOR_TEXT);
  render_text(renderer, font, "Te poti intoarce mereu la meniu, apasand ESC.", x, y + 4 * line_height, COLOR_TEXT);
  render_text(renderer, font, "Apasa ESC pentru a te intoarce in meniu.", x, y + 6 * line_height, COLOR_TEXT);

  SDL_RenderPresent(renderer);
}

void desen_fundal(SDL_Renderer* renderer) {
  set_render_color(renderer, COLOR_BACKGROUND);
  SDL_RenderClear(renderer);
}

void afiseaza_documentatie_parte(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size, int parte, Buton back_btn, int mouse_x, int mouse_y, StareJoc* stare) {
  set_render_color(renderer, COLOR_BACKGROUND);
  SDL_RenderClear(renderer);

  char linii[100][256];
  const char* filename = (parte == 1) ? "data/documentatie_partea1.txt" : "data/documentatie_partea2.txt";
  int nr = citeste_documentatie(filename, linii, 100);

  static int scroll_x_offset = 0;
  static bool dragging = false;
  static int drag_offset_x = 0;
  const int max_scroll = 500; // presupunem latime continut ~800 + scroll

  int bar_y = win_size.h - 40;
  int bar_w = win_size.w - 100;
  int thumb_w = (bar_w * bar_w) / (bar_w + max_scroll);
  int thumb_x = 50 + (scroll_x_offset * (bar_w - thumb_w)) / max_scroll;
  SDL_Rect thumb = {thumb_x, bar_y, thumb_w, 10};

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
      if (event.type == SDL_MOUSEBUTTONDOWN) {
          if (mouse_over(back_btn.rect, mouse_x, mouse_y)) {
              scroll_x_offset = 0;
              *stare = MENIU;
              return;
          }
          if (event.button.button == SDL_BUTTON_LEFT && mouse_over(thumb, mouse_x, mouse_y)) {
              dragging = true;
              drag_offset_x = mouse_x - thumb_x;
          }
      } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
          dragging = false;
      } else if (event.type == SDL_MOUSEMOTION && dragging) {
          int new_thumb_x = event.motion.x - drag_offset_x;
          if (new_thumb_x < 50) new_thumb_x = 50;
          if (new_thumb_x > 50 + bar_w - thumb_w) new_thumb_x = 50 + bar_w - thumb_w;

          scroll_x_offset = ((new_thumb_x - 50) * max_scroll) / (bar_w - thumb_w);
      } else if (event.type == SDL_MOUSEWHEEL) {
          scroll_x_offset -= event.wheel.x * 20; // scroll orizontal
          if (scroll_x_offset < 0) scroll_x_offset = 0;
          if (scroll_x_offset > max_scroll) scroll_x_offset = max_scroll;
      }
  }

  int y = 50;
  for (int i = 0; i < nr; i++, y += 30) {
      render_text(renderer, font, linii[i] + scroll_x_offset, 50, y, COLOR_TEXT);
  }

  // Scroll bar orizontal vizual
  SDL_Rect scrollbar_bg = {50, bar_y, bar_w, 10};
  set_render_color(renderer, COLOR_SCROLL_BG);
  SDL_RenderFillRect(renderer, &scrollbar_bg);

  set_render_color(renderer, COLOR_SCROLL_THUMB);
  SDL_RenderFillRect(renderer, &thumb);

  bool hover = mouse_over(back_btn.rect, mouse_x, mouse_y);
  render_button(renderer, font, back_btn, hover);
  SDL_RenderPresent(renderer);
}

void afiseaza_documentatie_select(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size, Buton but1, Buton but2, int mouse_x, int mouse_y) {
  set_render_color(renderer, COLOR_BACKGROUND);
  SDL_RenderClear(renderer);

  render_text(renderer, font, "Alege partea pentru documentatie:", win_size.w * 0.1f, win_size.h * 0.1f, COLOR_TEXT);

  but1.rect = get_scaled_rect(win_size, (RelativeRect){0.1f, 0.3f, 0.3f, 0.08f});
  but2.rect = get_scaled_rect(win_size, (RelativeRect){0.1f, 0.45f, 0.3f, 0.08f});

  bool hover1 = mouse_over(but1.rect, mouse_x, mouse_y);
  bool hover2 = mouse_over(but2.rect, mouse_x, mouse_y);
  render_button(renderer, font, but1, hover1);
  render_button(renderer, font, but2, hover2);


  SDL_RenderPresent(renderer);
}

SDL_Rect get_scaled_rect(AbsoluteSize window, RelativeRect rel) {
  // Calculam latimea si inaltimea scalata
  int w = window.w * rel.w;
  int h = window.h * rel.h;

  // Aplicam limite minime si maxime
  if (w < BTN_MIN_W) w = BTN_MIN_W;
  if (w > BTN_MAX_W) w = BTN_MAX_W;
  if (h < BTN_MIN_H) h = BTN_MIN_H;
  if (h > BTN_MAX_H) h = BTN_MAX_H;

  SDL_Rect rect;

  if (rel.x == CENTER_X)
    rect.x = window.w * rel.x - w / 2;
  else
    rect.x = window.w * rel.x;

  rect.y = window.h * rel.y;
  rect.w = w;
  rect.h = h;
  return rect;
}