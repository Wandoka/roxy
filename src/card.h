#pragma once
#include <wchar.h>

typedef struct Card {
  wchar_t japanese[200];
  wchar_t meaning[200];
  char addition_date[30];
} Card;

