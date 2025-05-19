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

#ifdef _WIN32
  #define FONT_PATH_BASE "../assets/fonts/"
#else
  #define FONT_PATH_BASE "assets/fonts/"
#endif

TTF_Font *fonts[FONT_VARIANTS + FONT_TITLE_VARIANTS];

const int font_sizes[FONT_VARIANTS + FONT_TITLE_VARIANTS] = {12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56};

void loadFonts() {
    for(int i = 0; i < FONT_VARIANTS + FONT_TITLE_VARIANTS; i++) {
        char path[256];
        snprintf(path, sizeof(path), "%sDejaVuSans.ttf", FONT_PATH_BASE);
        fonts[i] = TTF_OpenFont(path, font_sizes[i]);
        
        if(!fonts[i]) {
            printf("Eroare la incarcarea fontului %d: %s\n", font_sizes[i], TTF_GetError());
            exit(1);
        }
    }
}

void unloadFonts() {
    for(int i = 0; i < FONT_VARIANTS + FONT_TITLE_VARIANTS; i++) {
        if(fonts[i]) {
            TTF_CloseFont(fonts[i]);
        }
    }
}

TTF_Font* getFontForWindow(AbsoluteSize win_size) {
  float base_w = 1920.0f;
  float base_h = 1080.0f;
  float scale_x = win_size.w / base_w;
  float scale_y = win_size.h / base_h;
  float scale = (scale_x + scale_y) / 2.0f;

  int index = (int)((scale - 1.0f) * (FONT_VARIANTS / 2) + (FONT_VARIANTS / 2));

  if(index < 0) index = 0;
  if(index >= FONT_VARIANTS) index = FONT_VARIANTS - 1;

  return fonts[index];
}

TTF_Font* getFontForWindowTitle(AbsoluteSize win_size) {
  float base_w = 1280.0f;
  float base_h = 720.0f;
  float scale_x = win_size.w / base_w;
  float scale_y = win_size.h / base_h;
  float scale = (scale_x + scale_y) / 2.0f;

  int index = (int)((scale - 1.0f) * (FONT_VARIANTS / 2) + (FONT_VARIANTS / 2));

  if(index < 0) index = 0;
  if(index >= FONT_VARIANTS) index = FONT_VARIANTS - 1;

  return fonts[index + 2];
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

void render_text_button(SDL_Renderer* renderer, TTF_Font* font, Buton buton, const char* text, SDL_Color color) {
  SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int text_x = buton.rect.x + (buton.rect.w - surface->w) / 2;
  int text_y = buton.rect.y + (buton.rect.h - surface->h) / 2;
  SDL_Rect dst = {text_x, text_y, surface->w, surface->h};
  SDL_FreeSurface(surface);
  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}

void render_centered_wrapped_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Rect rect, SDL_Color color) {
  const int max_width = rect.w;
  char buffer[2048];
  strncpy(buffer, text, sizeof(buffer) - 1);
  buffer[sizeof(buffer) - 1] = '\0';

  char* line = strtok(buffer, " ");
  char current_line[1024] = "";
  int y = rect.y;

  while(line) {
    char temp[1024];
    snprintf(temp, sizeof(temp), "%s%s%s", current_line[0] ? current_line : "", current_line[0] ? " " : "", line);

    int w;
    TTF_SizeText(font, temp, &w, NULL);
    if(w > max_width && current_line[0]) {
      // afiseaza current_line
      int text_w, text_h;
      TTF_SizeText(font, current_line, &text_w, &text_h);
      int x = rect.x + (rect.w - text_w) / 2;

      SDL_Surface* surf = TTF_RenderText_Solid(font, current_line, color);
      SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
      SDL_Rect dst = {x, y, text_w, text_h};
      SDL_RenderCopy(renderer, tex, NULL, &dst);
      SDL_DestroyTexture(tex);
      SDL_FreeSurface(surf);

      y += text_h;
      current_line[0] = '\0';
    }

    strncat(current_line, current_line[0] ? " " : "", sizeof(current_line) - strlen(current_line) - 1);
    strncat(current_line, line, sizeof(current_line) - strlen(current_line) - 1);

    line = strtok(NULL, " ");
  }

  // Ultima linie
  if(current_line[0]) {
    int text_w, text_h;
    TTF_SizeText(font, current_line, &text_w, &text_h);
    int x = rect.x + (rect.w - text_w) / 2;

    SDL_Surface* surf = TTF_RenderText_Solid(font, current_line, color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect dst = {x, y, text_w, text_h};
    SDL_RenderCopy(renderer, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
  }
}

void render_button(SDL_Renderer* renderer, TTF_Font* font, Buton buton, bool hover) {
  SDL_Color color_background = hover ? COLOR_BUTTON_HOVER : COLOR_BUTTON_NORMAL;
  set_render_color(renderer, color_background);
  SDL_Rect r = buton.rect;
  SDL_RenderFillRect(renderer, &r);

  render_text_button(renderer, font, buton, buton.text, COLOR_WHITE);
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
  int x = win_size.w * 0.3f;
  int y = line_height * 5;

  render_text(renderer, font, "Scopul jocului este sa ghicesti cuvantul.", x, y, COLOR_TEXT);
  render_text(renderer, font, "Ai 5 incercari gresite pentru fiecare nivel.", x, y + line_height, COLOR_TEXT);
  render_text(renderer, font, "Daca gresesti, ramai pe acelasi nivel.", x, y + 2 * line_height, COLOR_TEXT);
  render_text(renderer, font, "Daca te intorci la meniu, progresul se pierde.", x, y + 3 * line_height, COLOR_TEXT);
  render_text(renderer, font, "Poti apasa SPACE sau ENTER pentru a continua.", x, y + 4 * line_height, COLOR_TEXT);
  render_text(renderer, font, "Poti consulta si sectiunea de notiuni teoretice din meniu.", x, y + 5 * line_height, COLOR_TEXT);
  render_text(renderer, font, "Apasa ESC pentru a te intoarce in meniu.", x, y + 6 * line_height, COLOR_TEXT);

  SDL_RenderPresent(renderer);
}

void desen_fundal(SDL_Renderer* renderer) {
  set_render_color(renderer, COLOR_BACKGROUND);
  SDL_RenderClear(renderer);
}

void afiseaza_documentatie_parte(SDL_Renderer* renderer, TTF_Font* font, TTF_Font* font_title, AbsoluteSize win_size, int parte, Buton back_btn, Buton before_btn, Buton next_btn, int mouse_x, int mouse_y, StareJoc* stare) {
  static int index = 0;
  char titluri[100][128];
  char texte[100][1024];

  const char* filename = (parte == 1) ? "data/documentatie_partea1.json" : "data/documentatie_partea2.json";
  int nr = citeste_documentatie_din_json(filename, titluri, texte, 100);

  if(index >= nr) index = nr - 1;
  if(index < 0) index = 0;

  font = getFontForWindow(win_size);
  set_render_color(renderer, COLOR_BACKGROUND);
  SDL_RenderClear(renderer);

  // Titlu centrat sus
  render_text_relative(renderer, font_title, win_size, titluri[index], (RelativePos){0.5f, 0.1f}, COLOR_TEXT);

  // Text word-wrapped
  SDL_Rect text_area = {
    win_size.w * 0.1f,
    win_size.h * 0.3f,
    win_size.w * 0.8f,
    win_size.h * 0.6f
  };
  render_centered_wrapped_text(renderer, font, texte[index], text_area, COLOR_TEXT);

  // === Actualizeaza pozitiile butoanelor inainte de desenare ===
  back_btn.rect = get_scaled_rect(win_size, (RelativeRect){0.5f, 0.9f, 0.2f, 0.07f});
  before_btn.rect = get_scaled_rect(win_size, (RelativeRect){0.15f, 0.9f, 0.2f, 0.07f});
  next_btn.rect = get_scaled_rect(win_size, (RelativeRect){0.85f, 0.9f, 0.2f, 0.07f});

  // Hover detection
  bool hover_inapoi = mouse_over(back_btn.rect, mouse_x, mouse_y);
  bool hover_anterior = mouse_over(before_btn.rect, mouse_x, mouse_y);
  bool hover_urmator = mouse_over(next_btn.rect, mouse_x, mouse_y);

  render_button(renderer, font, back_btn, hover_inapoi);
  render_button(renderer, font, before_btn, hover_anterior);
  render_button(renderer, font, next_btn, hover_urmator);

  SDL_RenderPresent(renderer);

  // === Event loop ===
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      *stare = IESIRE;
      return;
    }
    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
      index = 0;
      *stare = DOCUMENTATIE_SELECT;
      return;
    }

    if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
      if(mouse_over(back_btn.rect, mouse_x, mouse_y)) {
        index = 0;
        *stare = DOCUMENTATIE_SELECT;
        return;
      }
      if(mouse_over(before_btn.rect, mouse_x, mouse_y)) {
        if(index > 0) index--;
      }
      if(mouse_over(next_btn.rect, mouse_x, mouse_y)) {
        if(index < nr - 1) index++;
      }
    }
  }
}

void afiseaza_documentatie_select(SDL_Renderer* renderer, TTF_Font* font, AbsoluteSize win_size, Buton* but1, Buton* but2, int mouse_x, int mouse_y) {
  set_render_color(renderer, COLOR_BACKGROUND);
  SDL_RenderClear(renderer);

  render_text_relative(renderer, font, win_size, "Alege partea pentru notiuni:", (RelativePos){0.5f, 0.15f}, COLOR_TEXT);

  but1->rect = get_scaled_rect(win_size, (RelativeRect){0.5f, 0.3f, 0.3f, 0.08f});
  but2->rect = get_scaled_rect(win_size, (RelativeRect){0.5f, 0.45f, 0.3f, 0.08f});

  bool hover1 = mouse_over(but1->rect, mouse_x, mouse_y);
  bool hover2 = mouse_over(but2->rect, mouse_x, mouse_y);
  render_button(renderer, font, *but1, hover1);
  render_button(renderer, font, *but2, hover2);

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

  rect.x = window.w * rel.x - w / 2;
  rect.y = window.h * rel.y - h / 2;
  rect.w = w;
  rect.h = h;
  return rect;
}

SDL_Rect get_scaled_img(AbsoluteSize window, RelativeRect rel) {
  // pastram ratia latime/inaltime, ne luam dupa inaltime
  int h = window.h * rel.h;
  int w = h * rel.w / rel.h;

  // Aplicam limite minime si maxime
  if (w < IMG_MIN_W) w = IMG_MIN_W;
  if (w > IMG_MAX_W) w = IMG_MAX_W;
  if (h < IMG_MIN_H) h = IMG_MIN_H;
  if (h > IMG_MAX_H) h = IMG_MAX_H;

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