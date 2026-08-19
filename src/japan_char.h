#pragma once
#include <wchar.h>

typedef struct JapanChar {
  int id;
  wchar_t symbol[4];
  char romaji[10];
  int row;
  int column;
  char type[10];
  char subtype[10];
  int can_sokuon;
  int can_yoon;
} JapanChar;


void wstring_from_japan_chars(int n, JapanChar japanString[n], int m, wchar_t wstring[m], int *wlen);
int can_have_sokuon_before(JapanChar *c);
