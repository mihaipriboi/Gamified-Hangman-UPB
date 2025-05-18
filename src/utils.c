#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

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