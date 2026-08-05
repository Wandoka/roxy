#pragma once
#include <wchar.h>

void fill_full_hiragana_table();

typedef struct Hiragana {
  wchar_t *symbol;
  char *romaji;
  int row;
  int column;
  char *type;
  int can_sokuon;
} Hiragana;
