#pragma once
#include "japan_char.h"
#include "card.h"
#include "fsrs_algorithm.h"
#include "kana_trainer.h"

void select_kana_rows(int n, JapanChar japanString[n], int *found_rows, int up_row, int down_row, DAKUEN_HANDAKUEN dakuen_handakuen, KANA_TYPE kana_type);
void select_cards(int n, Card listOfCards[n], int *found_cards);
void insert_symbol_training_history(int JapanChar_id, int failed_attempts, int spent_time_ms);
void insert_card_training_history(Card *card, FSRS_GRADE grade, int failed_symbols, int without_hint, int spent_time_ms);
void update_card_FSRS_data(Card* c);
void update_card_stepsUntilLearned_data(Card* c);
void update_card_extra_data(Card* c);
int select_lowest_R_card(Card *c);
int select_random_new_card(Card *c);
int select_random_learned_card(Card *c);
int count_new_cards();
