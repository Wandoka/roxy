#include "card_rememberence_grading.h"


FSRS_GRADE grade_card_rememberence(int failed_symbols, int without_hint, int spent_time_ms) {
  if(!without_hint) return FSRS_FORGET;
  else if(failed_symbols == 0 && spent_time_ms <= 20000) {
    return FSRS_RECALL_EASY;
  }
  else if(failed_symbols >= 5) {
    return FSRS_RECALL_HARD;
  }
  else {
    return FSRS_RECALL_GOOD;
  }
}
