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

int citeste_documentatie_din_json(const char* filename, char titluri[][128], char texte[][1024], int max_elem) {
  FILE* f = fopen(filename, "rb");
  if (!f) {
    printf("Nu s-a putut deschide fisierul: %s\n", filename);
    return 0;
  }

  fseek(f, 0, SEEK_END);
  long len = ftell(f);
  fseek(f, 0, SEEK_SET);

  char* data = (char*)malloc(len + 1);
  fread(data, 1, len, f);
  data[len] = '\0';
  fclose(f);

  cJSON* root = cJSON_Parse(data);
  free(data);

  if (!root) {
    printf("Eroare la parsarea JSON-ului.\n");
    return 0;
  }

  cJSON* docs = cJSON_GetObjectItemCaseSensitive(root, "documentation");
  if (!cJSON_IsArray(docs)) {
    printf("Campul 'documentation' nu este un array.\n");
    cJSON_Delete(root);
    return 0;
  }

  int count = 0;
  cJSON* doc;
  cJSON_ArrayForEach(doc, docs) {
    if (count >= max_elem) break;

    cJSON* title = cJSON_GetObjectItemCaseSensitive(doc, "title");
    cJSON* text = cJSON_GetObjectItemCaseSensitive(doc, "text");

    if (cJSON_IsString(title) && cJSON_IsString(text)) {
      strncpy(titluri[count], title->valuestring, 127);
      titluri[count][127] = '\0';

      strncpy(texte[count], text->valuestring, 1023);
      texte[count][1023] = '\0';

      count++;
    }
  }

  cJSON_Delete(root);
  return count;
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