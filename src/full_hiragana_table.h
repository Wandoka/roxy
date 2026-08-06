#pragma once
#include <wchar.h>
void fill_full_hiragana_table();

typedef struct Hiragana {
  wchar_t symbol[4];
  char romaji[6];
  int row;
  int column;
  char type[10];
  int can_sokuon;
} Hiragana;
