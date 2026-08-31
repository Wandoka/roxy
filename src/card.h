#pragma once
#include <wchar.h>

typedef struct Card {
  int id;
  wchar_t back[200];
  wchar_t front[200];
  double FSRS_Stability;
  double FSRS_Difficulty;
  int has_FSRS_data;
  char when_added[128];
  int last_review_unix_time;
} Card;

