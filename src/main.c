// Proiect: Hangman EDU
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// === HEADERE ===
#include "style.h"
#include "types.h"
#include "list.h"
#include "ui.h"
#include "utils.h"
#include "game.h"

int main(int argc, char* argv[]) {
    (void)argc;  // Eliminare avertizari de compilare
    (void)argv;

    AbsoluteSize win_size = {DEFAULT_WIDTH, DEFAULT_HEIGHT};

    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) return 1;
    SDL_Window* window = SDL_CreateWindow(
      "Hangman EDU",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      win_size.w, win_size.h,
      SDL_WINDOW_RESIZABLE
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("assets/fonts/DejaVuSans.ttf", 24);
    if (!font) return 1;

    Buton butoane[] = {
        {{300, 120, 200, 50}, "Partea I"},
        {{300, 190, 200, 50}, "Partea II"},
        {{300, 260, 200, 50}, "Instructiuni"},
        {{300, 330, 200, 50}, "Documentatie"},
        {{300, 400, 200, 50}, "Iesire"}
    };

    Buton doc_but1 = {{100, 180, 200, 50}, "Partea I"};
    Buton doc_but2 = {{100, 260, 200, 50}, "Partea II"};
    Buton back_btn = {{100, 460, 250, 50}, "Inapoi la meniu"};

    StareJoc stare = MENIU;
    bool running = true;
    SDL_Event event;
    SDL_StartTextInput();

    while (running) {
      int mouse_x, mouse_y;
      SDL_GetMouseState(&mouse_x, &mouse_y);
  
      // Verificare dimensiuni fereastra
      SDL_GetRendererOutputSize(renderer, &win_size.w, &win_size.h);
  
      while (SDL_PollEvent(&event)) {
          if (event.type == SDL_QUIT) running = false;
  
          switch (stare) {
              case MENIU:
                  if (event.type == SDL_MOUSEBUTTONDOWN) {
                      for (int i = 0; i < 5; i++) {
                          if (mouse_over(butoane[i].rect, mouse_x, mouse_y)) {
                              if (i == 0) stare = PARTEA_I;
                              if (i == 1) stare = PARTEA_II;
                              if (i == 2) stare = INSTRUCTIUNI;
                              if (i == 3) stare = DOCUMENTATIE_SELECT;
                              if (i == 4) stare = IESIRE;
                          }
                      }
                  }
                  break;
  
              case DOCUMENTATIE_SELECT:
                  if (event.type == SDL_MOUSEBUTTONDOWN) {
                      if (mouse_over(doc_but1.rect, mouse_x, mouse_y)) stare = DOCUMENTATIE_PARTE1;
                      if (mouse_over(doc_but2.rect, mouse_x, mouse_y)) stare = DOCUMENTATIE_PARTE2;
                  }
                  break;
  
              case DOCUMENTATIE_PARTE1:
              case DOCUMENTATIE_PARTE2:
                  if (event.type == SDL_MOUSEBUTTONDOWN) {
                      if (mouse_over(back_btn.rect, mouse_x, mouse_y)) stare = MENIU;
                  }
                  break;
  
              case INSTRUCTIUNI:
                  if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                      stare = MENIU;
                  }
                  break;
  
              default:
                  break;
          }
      }
  
      switch (stare) {
          case MENIU:
              afiseaza_meniu(renderer, font, win_size, butoane, mouse_x, mouse_y);
              break;
  
          case INSTRUCTIUNI:
              afiseaza_instructiuni(renderer, font, win_size);
              break;
  
          case DOCUMENTATIE_SELECT:
              afiseaza_documentatie_select(renderer, font, win_size, doc_but1, doc_but2, mouse_x, mouse_y);
              break;
  
          case DOCUMENTATIE_PARTE1:
              afiseaza_documentatie_parte(renderer, font, win_size, 1, back_btn, mouse_x, mouse_y, &stare);
              break;
  
          case DOCUMENTATIE_PARTE2:
              afiseaza_documentatie_parte(renderer, font, win_size, 2, back_btn, mouse_x, mouse_y, &stare);
              break;
  
          case PARTEA_I:
          case PARTEA_II: {
              const char* path = (stare == PARTEA_I) ? "data/intrebari_partea1.txt" : "data/intrebari_partea2.txt";
              Nod* lista = incarca_intrebari_din_fisier(path);
              Nod* curent = lista;
              while (curent && (stare == PARTEA_I || stare == PARTEA_II)) {
                  joc_pe_nivel(renderer, font, win_size, curent, &stare);
                  if ((stare == PARTEA_I || stare == PARTEA_II) && curent->next) {
                      curent = curent->next;
                  } else {
                      break;
                  }
              }
              elibereaza_lista(&lista);
              stare = MENIU;
              break;
          }
  
          case IESIRE:
              running = false;
              break;
      }
  }  

    SDL_StopTextInput();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}