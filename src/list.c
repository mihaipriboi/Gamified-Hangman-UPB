#include "types.h"
#include "cJSON.h"

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

Nod* incarca_intrebari_din_json(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if(!file) {
    printf("Eroare la deschiderea fisierului JSON: %s\n", filename);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long len = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *data = (char *)malloc(len + 1);
  fread(data, 1, len, file);
  data[len] = '\0';
  fclose(file);

  cJSON *json = cJSON_Parse(data);
  free(data);

  if(!json) {
    printf("Eroare la parsarea JSON-ului.\n");
    return NULL;
  }

  cJSON *questions = cJSON_GetObjectItemCaseSensitive(json, "questions");
  cJSON *question;

  Nod* cap = NULL;

  cJSON_ArrayForEach(question, questions) {
    cJSON *hint = cJSON_GetObjectItemCaseSensitive(question, "hint");
    cJSON *answer = cJSON_GetObjectItemCaseSensitive(question, "answer");

    if(cJSON_IsString(hint) && cJSON_IsString(answer)) {
      adauga_nod_final(&cap, hint->valuestring, answer->valuestring);
    }
  }

  cJSON_Delete(json);
  
  return cap;
}