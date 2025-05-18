// main.c
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define WIDTH 800
#define HEIGHT 600
#define MAX_GRESITE 5

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

Nod* creare_nod(const char* hint, const char* answer) {
    Nod* nod = (Nod*)malloc(sizeof(Nod));
    if (!nod) return NULL;
    strncpy(nod->hint, hint, sizeof(nod->hint));
    strncpy(nod->answer, answer, sizeof(nod->answer));
    nod->prev = NULL;
    nod->next = NULL;
    return nod;
}

void adauga_nod_final(Nod** cap, const char* hint, const char* answer) {
    Nod* nou = creare_nod(hint, answer);
    if (!*cap) {
        *cap = nou;
    } else {
        Nod* temp = *cap;
        while (temp->next) temp = temp->next;
        temp->next = nou;
        nou->prev = temp;
    }
}

void elibereaza_lista(Nod** cap) {
    while (*cap) {
        Nod* tmp = *cap;
        *cap = (*cap)->next;
        free(tmp);
    }
}

Nod* incarca_intrebari_din_fisier(const char* nume_fisier) {
    FILE* f = fopen(nume_fisier, "r");
    if (!f) return NULL;

    Nod* cap = NULL;
    char linie[512];
    while (fgets(linie, sizeof(linie), f)) {
        linie[strcspn(linie, "\n")] = 0;
        char* separator = strchr(linie, '|');
        if (separator) {
            *separator = '\0';
            char* hint = linie;
            char* answer = separator + 1;
            adauga_nod_final(&cap, hint, answer);
        }
    }
    fclose(f);
    return cap;
}

// Nod* creare_lista_demo_parte1() {
//     Nod* cap = NULL;
//     adauga_nod_final(&cap, "Este procesul de optimizare pentru motoare de cautare", "seo");
//     adauga_nod_final(&cap, "Element multimedia utilizat pe web", "video");
//     adauga_nod_final(&cap, "Platforma deschisa pentru colaborare", "github");
//     adauga_nod_final(&cap, "Instrument de analiza pentru site-uri", "analytics");
//     adauga_nod_final(&cap, "Format universal de fisier", "pdf");
//     return cap;
// }

// Nod* creare_lista_demo_parte2() {
//     Nod* cap = NULL;
//     adauga_nod_final(&cap, "Procesul de transformare a datelor brute", "cleaning");
//     adauga_nod_final(&cap, "Tehnica de combinare a seturilor de date", "merging");
//     adauga_nod_final(&cap, "Reprezentarea grafica a datelor", "chart");
//     adauga_nod_final(&cap, "Fisiere CSV sunt exemple de date __", "structurate");
//     adauga_nod_final(&cap, "Evaluarea datelor cu privire la moralitate", "etica");
//     return cap;
// }

typedef struct {
    SDL_Rect rect;
    const char* text;
} Buton;

void render_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

void update_display_word(const char* word, const bool guessed[26], char* display) {
    int len = strlen(word);
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (isalpha(word[i]) && guessed[tolower(word[i]) - 'a']) {
            display[j++] = word[i];
        } else {
            display[j++] = '_';
        }
        display[j++] = ' ';
    }
    display[j - 1] = '\0';
}

void render_button(SDL_Renderer* renderer, TTF_Font* font, Buton buton, bool hover) {
    SDL_Color bg = hover ? (SDL_Color){140, 90, 40, 255} : (SDL_Color){180, 140, 100, 255}; // maro închis la hover
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, 255);
    SDL_Rect r = buton.rect;
    SDL_RenderFillRect(renderer, &r);

    SDL_Color white = {255, 255, 255, 255};
    int text_x = buton.rect.x + 20;
    int text_y = buton.rect.y + 10;
    render_text(renderer, font, buton.text, text_x, text_y, white);
}


void render_simple_button(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect rect, const char* text, bool hover) {
    SDL_Color bg = hover ? (SDL_Color){100, 100, 255, 255} : (SDL_Color){70, 70, 70, 255};
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_Color white = {255, 255, 255, 255};
    int text_x = rect.x + 20;
    int text_y = rect.y + 10;
    render_text(renderer, font, text, text_x, text_y, white);
}

bool mouse_over(SDL_Rect rect, int mx, int my) {
    return mx >= rect.x && mx <= rect.x + rect.w && my >= rect.y && my <= rect.y + rect.h;
}

void afiseaza_meniu(SDL_Renderer* renderer, TTF_Font* font, Buton butoane[], int mouse_x, int mouse_y) {
    SDL_SetRenderDrawColor(renderer, 245, 235, 220, 255); // crem deschis
    SDL_RenderClear(renderer);

    SDL_Color titleColor = {80, 40, 0, 255};
    render_text(renderer, font, "=== HANGMAN EDU ===", 250, 60, titleColor);

    for (int i = 0; i < 5; i++) {
        bool hover = mouse_over(butoane[i].rect, mouse_x, mouse_y);
        render_button(renderer, font, butoane[i], hover);
    }

    SDL_RenderPresent(renderer);
}

void afiseaza_instructiuni(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 245, 235, 220, 255); // crem deschis
    SDL_RenderClear(renderer);

    SDL_Color text = {80, 40, 0, 255};
    render_text(renderer, font, "Scopul jocului este sa ghicesti cuvintele din definitii.", 100, 150, text);
    render_text(renderer, font, "Ai 5 incercari gresite pentru fiecare nivel.", 100, 200, text);
    render_text(renderer, font, "Daca gresesti, ramai pe acelasi nivel.", 100, 250, text);
    render_text(renderer, font, "Daca te intorci la meniu, progresul se pierde.", 100, 300, text);
    render_text(renderer, font, "Te poti intoarce mereu la meniu, apasand ESC.", 100, 350, text);
    render_text(renderer, font, "Apasa ESC pentru a te intoarce in meniu.", 100, 450, text);

    SDL_RenderPresent(renderer);
}

void desen_fundal(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 245, 235, 220, 255); // crem deschis
    SDL_RenderClear(renderer);
}

bool is_word_guessed(const char* word, const char* display) {
    char stripped[128];
    int j = 0;
    for (int i = 0; display[i]; i++) {
        if (display[i] != ' ') {
            stripped[j++] = display[i];
        }
    }
    stripped[j] = '\0';
    return strcmp(word, stripped) == 0;

}

int citeste_documentatie(const char* filename, char linii[][256], int max_linii) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Nu s-a putut deschide fisierul: %s\n", filename);
        return 0;
    }

    int count = 0;
    while (fgets(linii[count], 256, f) && count < max_linii) {
        // Scoatem caracterul '\n'
        linii[count][strcspn(linii[count], "\n")] = '\0';
        count++;
    }
    fclose(f);
    return count;
}


void afiseaza_documentatie_parte(SDL_Renderer* renderer, TTF_Font* font, int parte, Buton back_btn, int mouse_x, int mouse_y, StareJoc* stare) {
    SDL_SetRenderDrawColor(renderer, 245, 235, 220, 255);
    SDL_RenderClear(renderer);

    SDL_Color text = {80, 40, 0, 255};
    char linii[100][256];
    const char* filename = (parte == 1) ? "data/documentatie_partea1.txt" : "data/documentatie_partea2.txt";
    int nr = citeste_documentatie(filename, linii, 100);

    static int scroll_x_offset = 0;
    static bool dragging = false;
    static int drag_offset_x = 0;
    const int max_scroll = 500; // presupunem latime continut ~800 + scroll

    int bar_y = HEIGHT - 40;
    int bar_w = WIDTH - 100;
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
        render_text(renderer, font, linii[i] + scroll_x_offset, 50, y, text);
    }

    // Scroll bar orizontal vizual
    SDL_Rect scrollbar_bg = {50, bar_y, bar_w, 10};
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &scrollbar_bg);

    SDL_SetRenderDrawColor(renderer, 120, 80, 40, 255);
    SDL_RenderFillRect(renderer, &thumb);

    bool hover = mouse_over(back_btn.rect, mouse_x, mouse_y);
    render_button(renderer, font, back_btn, hover);
    SDL_RenderPresent(renderer);
}



void afiseaza_documentatie_select(SDL_Renderer* renderer, TTF_Font* font, Buton but1, Buton but2, int mouse_x, int mouse_y) {
    SDL_SetRenderDrawColor(renderer, 245, 235, 220, 255);
    SDL_RenderClear(renderer);

    SDL_Color title = {80, 40, 0, 255};
    render_text(renderer, font, "Alege partea pentru documentatie:", 100, 80, title);

    bool hover1 = mouse_over(but1.rect, mouse_x, mouse_y);
    bool hover2 = mouse_over(but2.rect, mouse_x, mouse_y);
    render_button(renderer, font, but1, hover1);
    render_button(renderer, font, but2, hover2);

    SDL_RenderPresent(renderer);
}




void joc_pe_nivel(SDL_Renderer* renderer, TTF_Font* font, Nod* nivel, StareJoc* stare) {
    bool guessed[26] = {false};
    int gresite = 0;
    char display[128];

    SDL_StartTextInput();
    update_display_word(nivel->answer, guessed, display);
    SDL_Event event;
    bool in_nivel = true;
    bool corect = false;
    bool pierdut = false;

    Buton next_btn = {{50, 460, 220, 50}, "Nivelul urmator"};
    Buton retry_btn = {{50, 460, 200, 50}, "Reincearca"};
    Buton menu_btn  = {{270, 460, 250, 50}, "Inapoi la meniu"};

    while (in_nivel) {
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *stare = IESIRE;
                return;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                *stare = MENIU;
                return;
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

        SDL_SetRenderDrawColor(renderer, 245, 235, 220, 255); // crem deschis fundal joc
        SDL_RenderClear(renderer);

        SDL_Color maro = {80, 40, 0, 255};
        render_text(renderer, font, "Hint:", 50, 30, maro);
        render_text(renderer, font, nivel->hint, 50, 70, maro);

        char path[64];
        if (gresite < MAX_GRESITE) {
            snprintf(path, sizeof(path), "assets/hangman_%d.bmp", gresite);
        } else {
            snprintf(path, sizeof(path), "assets/game_over.bmp");
        }

        SDL_Surface* image = SDL_LoadBMP(path);
        if (image) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
            SDL_FreeSurface(image);
            SDL_Rect imgRect = {WIDTH / 2 - 100, 120, 200, 200};
            SDL_RenderCopy(renderer, texture, NULL, &imgRect);
            SDL_DestroyTexture(texture);
        }

        render_text(renderer, font, display, 50, 350, maro);

        if (is_word_guessed(nivel->answer, display)) {
            corect = true;
            render_text(renderer, font, "Corect!", 50, 410, maro);
            bool hover = mouse_over(next_btn.rect, mouse_x, mouse_y);
            render_button(renderer, font, next_btn, hover);
        } else if (gresite >= MAX_GRESITE) {
            pierdut = true;
            render_text(renderer, font, "Ai pierdut nivelul.", 50, 410, maro);
            bool hover1 = mouse_over(retry_btn.rect, mouse_x, mouse_y);
            bool hover2 = mouse_over(menu_btn.rect, mouse_x, mouse_y);
            render_button(renderer, font, retry_btn, hover1);
            render_button(renderer, font, menu_btn, hover2);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
}

// === MAIN CU INTEGRARE LISTE ===

int main(int argc, char* argv[]) {
    (void)argc;  // Eliminare avertizari de compilare
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) return 1;
    SDL_Window* window = SDL_CreateWindow("Hangman EDU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
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

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (stare == MENIU && event.type == SDL_MOUSEBUTTONDOWN) {
                for (int i = 0; i < 5; i++) {
                    if (mouse_over(butoane[i].rect, mouse_x, mouse_y)) {
                        if (i == 0) stare = PARTEA_I;
                        if (i == 1) stare = PARTEA_II;
                        if (i == 2) stare = INSTRUCTIUNI;
                        if (i == 3) stare = DOCUMENTATIE_SELECT;
                        if (i == 4) stare = IESIRE;
                    }
                }
            } else if (stare == DOCUMENTATIE_SELECT && event.type == SDL_MOUSEBUTTONDOWN) {
                if (mouse_over(doc_but1.rect, mouse_x, mouse_y)) stare = DOCUMENTATIE_PARTE1;
                if (mouse_over(doc_but2.rect, mouse_x, mouse_y)) stare = DOCUMENTATIE_PARTE2;
            } else if ((stare == DOCUMENTATIE_PARTE1 || stare == DOCUMENTATIE_PARTE2) && event.type == SDL_MOUSEBUTTONDOWN) {
                if (mouse_over(back_btn.rect, mouse_x, mouse_y)) stare = MENIU;
            } else if (stare == INSTRUCTIUNI && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                stare = MENIU;
            }
        }

        if (stare == MENIU) {
            afiseaza_meniu(renderer, font, butoane, mouse_x, mouse_y);
        } else if (stare == INSTRUCTIUNI) {
            afiseaza_instructiuni(renderer, font);
        } else if (stare == DOCUMENTATIE_SELECT) {
            afiseaza_documentatie_select(renderer, font, doc_but1, doc_but2, mouse_x, mouse_y);
        } else if (stare == DOCUMENTATIE_PARTE1) {
            afiseaza_documentatie_parte(renderer, font, 1, back_btn, mouse_x, mouse_y, &stare);
        } else if (stare == DOCUMENTATIE_PARTE2) {
            afiseaza_documentatie_parte(renderer, font, 2, back_btn, mouse_x, mouse_y, &stare);
        } else if (stare == PARTEA_I || stare == PARTEA_II) {
            Nod* lista = (stare == PARTEA_I) ? incarca_intrebari_din_fisier("data/intrebari_partea1.txt") : incarca_intrebari_din_fisier("data/intrebari_partea2.txt");
            Nod* curent = lista;
            while (curent && (stare == PARTEA_I || stare == PARTEA_II)) {
                joc_pe_nivel(renderer, font, curent, &stare);
                if ((stare == PARTEA_I || stare == PARTEA_II) && curent->next) {
                    curent = curent->next;
                } else {
                    break;
                }
            }
            elibereaza_lista(&lista);
            stare = MENIU;
        } else if (stare == IESIRE) {
            running = false;
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
