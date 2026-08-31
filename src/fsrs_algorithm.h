#pragma once
#include "card.h"

typedef enum { 
  FSRS_FORGET      = 1, 
  FSRS_RECALL_HARD = 2, 
  FSRS_RECALL_GOOD = 3, 
  FSRS_RECALL_EASY = 4 
} FSRS_GRADE;

double fsrs_calc_R_recall_value(double t, double s);
double fsrs_days_till_repeat(double s, double r);
void fsrs_review(Card *c, int g);
