#pragma once
#include <wchar.h>

typedef enum {
    HIRAGANA = 0,
    KATAGANA  = 1,
    KANJI  = 2,
} Type;

typedef struct Hiragana {
  wchar_t symbol[4];
  char romaji[6];
  int row;
  int column;
  char type[10];
  int can_sokuon;
} Hiragana;

typedef struct Katagana {
} Katagana;
typedef struct Kanji {
} Kanji;


typedef struct JapanChar {
  Type type;
  Hiragana hiragana;
  Katagana katagana;
  Kanji kanji;
  int sokuon_before;
} JapanChar;


void wstring_from_japan_chars(int n, JapanChar japan_string[n], int m, wchar_t wstring[m], int *wlen);
void unite_hiragana_kanji_katagana(int n, Hiragana hiragana[n], int m, Katagana katagana[m], int k, Kanji kanji[k], int q, JapanChar japan_string[q], int *am);
int can_have_sokuon_before(JapanChar *c);
