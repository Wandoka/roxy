#pragma once
#include "japan_char.h"
#include "card.h"
#include "fsrs_algorithm.h"
#include "kana_trainer.h"

void select_kana_rows(int n, JapanChar japanString[n], int *found_rows, int up_row, int down_row, DAKUEN_HANDAKUEN dakuen_handakuen, KANA_TYPE kana_type);
void select_cards(int n, Card listOfCards[n], int *found_cards);
void insert_symbol_training_history(int JapanChar_id, int failed_attempts, int spent_time_ms);
void insert_card_training_FSRS_history(Card *card, FSRS_GRADE grade, int failed_symbols, int used_hint, int spent_time_ms, double FSRS_Stability_BEFORE, double FSRS_Difficulty_BEFORE, int grade_is_forced);
void update_card_FSRS_data(Card* c);
void update_card_stepsUntilLearned_data(Card* c);
void update_card_extra_data(Card* c);
int select_lowest_R_card(Card *c);
int select_random_new_card(Card *c, int n, int banned_ids[n]);
int select_random_learned_card(Card *c, int n, int banned_ids[n]);
int count_new_cards();
void add_new_card(int n, const wchar_t back[n], int m, const wchar_t front[m], int q, const char type[q]);
int find_card_by_wstring(Card* c, int n, const wchar_t wstring[n]);
void update_card_back(int id, int n, const wchar_t back[n]);
void update_card_front(int id, int n, const wchar_t front[n]);
void update_card_banished_state(int id, int banished_state);
void insert_card_training_history(Card *card, FSRS_GRADE grade, int failed_symbols, int used_hint, int spent_time_ms, int grade_is_forced, int suggestion_type);
