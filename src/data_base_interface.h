#pragma once
#include "japan_char.h"
#include "card.h"

void select_hiragana_rows(int n, JapanChar japanString[n], int *found_rows, int up_row, int down_row);
void select_card_rows(int n, Card listOfCards[n], int *found_rows);
void insert_symbol_training_statistic(int JapanChar_id, int failed_attempts, int spent_time_ms);
