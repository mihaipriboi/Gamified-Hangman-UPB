#include "types.h"

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