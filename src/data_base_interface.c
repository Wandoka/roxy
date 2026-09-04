#include "data_base_interface.h"
#include "data_base_query_templates.h"
#include "common.h"
#include "logger.h"
#include <string.h>
#include <time.h>

void select_kana_rows(int n, JapanChar japanString[n], int *found_rows, int up_row, int down_row, DAKUEN_HANDAKUEN dakuen_handakuen, KANA_TYPE kana_type) {
  const char *sql =
    " SELECT id, symbol, row, column, type, subtype, can_sokuon, can_yoon"
    " FROM JapanChars "
    " WHERE type = ? AND (row >= ? AND row <= ?)"
    " AND (1 = ? OR subtype = 'normal');"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  if(kana_type == HIRAGANA) {
    const char* str = "hiragana";
    sql_bind_text(stmt, 1, strlen(str), str);
  }
  else { //KATAKANA
    const char* str = "katakana";
    sql_bind_text(stmt, 1, strlen(str), str);
  }
  sql_bind_int(stmt, 2, up_row);
  sql_bind_int(stmt, 3, down_row);
  if(dakuen_handakuen == NO_DAKUEN_HANDAKUEN) {
    sql_bind_int(stmt, 4, 0);
  }
  else {
    sql_bind_int(stmt, 4, 1);
  }
  *found_rows = 0;
  for(int i = 0; i < n; ++i) {
    if(sqlite3_step(stmt) != SQLITE_ROW) break;
    JapanChar *h = &japanString[i];
    stmt_column_int(stmt,   0, &h->id);
    stmt_column_wtext(stmt, 1, ARRAY_SIZE(h->symbol), h->symbol);
    stmt_column_int(stmt,   2, &h->row);
    stmt_column_int(stmt,   3, &h->column);
    stmt_column_text(stmt,  4, ARRAY_SIZE(h->type), h->type);
    stmt_column_text(stmt,  5, ARRAY_SIZE(h->subtype), h->subtype);
    stmt_column_int(stmt,   6, &h->can_sokuon);
    stmt_column_int(stmt,   7, &h->can_yoon);
    ++*found_rows;
  }
  sqlite3_finalize(stmt);
}

int select_lowest_R_card(Card *c) {
  const char *sql =
    " SELECT id, back, front, FSRS_Stability, FSRS_Difficulty, has_FSRS_data, last_FSRS_review_unix_time, last_seen, steps_until_learned, times_seen,"
    "         ((? - last_FSRS_review_unix_time) / ?) / FSRS_Stability as R_approximation_for_sorting"
    " FROM Cards"
    " WHERE has_FSRS_data = 1 AND banished = 0"
    " ORDER BY R_approximation_for_sorting DESC"
    " LIMIT 1;"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_int(stmt, 1, time(NULL));
  sql_bind_double(stmt, 2, 60.0*60.0*24.0);
  if(sqlite3_step(stmt) != SQLITE_ROW) {
    LOG("failed to find card in function \"select_lowest_R_card\"");
    return -1;
  }

  stmt_column_int  (stmt,  0, &c->id);
  stmt_column_wtext(stmt,  1, ARRAY_SIZE(c->back), c->back);
  stmt_column_wtext(stmt,  2, ARRAY_SIZE(c->front), c->front);
  stmt_column_double(stmt,  3, &c->FSRS_Stability);
  stmt_column_double(stmt,  4, &c->FSRS_Difficulty);
  stmt_column_int(stmt,  5, &c->has_FSRS_data); 
  stmt_column_int(stmt,  6, &c->last_FSRS_review_unix_time); 
  stmt_column_text(stmt,  7, ARRAY_SIZE(c->last_seen), c->last_seen); 
  stmt_column_int(stmt,  8, &c->steps_until_learned); 
  stmt_column_int(stmt,  9, &c->times_seen);
  sqlite3_finalize(stmt);

  LOG_W(L"FOUND card in \"select_lowest_R_card\"\n");
  LOG_W(c->front);
  return 0;
}

int select_random_new_card(Card *c, int n, int banned_ids[n]) {
  const char *sql =
    " SELECT id, back, front, FSRS_Stability, FSRS_Difficulty, has_FSRS_data, last_FSRS_review_unix_time, last_seen, steps_until_learned, times_seen"
    " FROM Cards"
    " WHERE has_FSRS_data == 0 AND banished = 0 AND id NOT IN (?, ?, ?, ?, ?, ?, ?)"
    " ORDER BY RANDOM()"
    " LIMIT 1;"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  assert(n == 7);
  for(int i = 0; i < n; ++i) {
    sql_bind_int(stmt, i+1, banned_ids[i]);
  }
  if(sqlite3_step(stmt) != SQLITE_ROW) return -1;
  stmt_column_int  (stmt,  0, &c->id);
  stmt_column_wtext(stmt,  1, ARRAY_SIZE(c->back), c->back);
  stmt_column_wtext(stmt,  2, ARRAY_SIZE(c->front), c->front);
  stmt_column_double(stmt,  3, &c->FSRS_Stability);
  stmt_column_double(stmt,  4, &c->FSRS_Difficulty);
  stmt_column_int(stmt,  5, &c->has_FSRS_data); 
  stmt_column_int(stmt,  6, &c->last_FSRS_review_unix_time); 
  stmt_column_text(stmt,  7, ARRAY_SIZE(c->last_seen), c->last_seen); 
  stmt_column_int(stmt,  8, &c->steps_until_learned); 
  stmt_column_int(stmt,  9, &c->times_seen);
  sqlite3_finalize(stmt);
  return 0;
}

int count_new_cards() {
  const char *sql =
    " SELECT COUNT(*)"
    " FROM Cards"
    " WHERE has_FSRS_data == 0 and banished = 0;"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  if(sqlite3_step(stmt) != SQLITE_ROW) return -1;
  int result;
  stmt_column_int  (stmt,  0, &result);
  sqlite3_finalize(stmt);
  return result;
}

int select_random_learned_card(Card *c, int n, int banned_ids[n]) {
  const char *sql =
    " SELECT id, back, front, FSRS_Stability, FSRS_Difficulty, has_FSRS_data, last_FSRS_review_unix_time, last_seen, steps_until_learned, times_seen"
    " FROM Cards"
    " WHERE has_FSRS_data != 0 and banished = 0 AND id NOT IN (?, ?, ?, ?, ?, ?, ?)"
    " ORDER BY ABS(RANDOM()) * 1.0 / (FSRS_Difficulty * FSRS_Difficulty)"
    " LIMIT 1;";

  sqlite3_stmt *stmt = sql_prepare(sql);
  assert(n == 7);
  for(int i = 0; i < n; ++i) {
    sql_bind_int(stmt, i+1, banned_ids[i]);
  }
  if(sqlite3_step(stmt) != SQLITE_ROW) return -1;
  stmt_column_int  (stmt,  0, &c->id);
  stmt_column_wtext(stmt,  1, ARRAY_SIZE(c->back), c->back);
  stmt_column_wtext(stmt,  2, ARRAY_SIZE(c->front), c->front);
  stmt_column_double(stmt,  3, &c->FSRS_Stability);
  stmt_column_double(stmt,  4, &c->FSRS_Difficulty);
  stmt_column_int(stmt,  5, &c->has_FSRS_data); 
  stmt_column_int(stmt,  6, &c->last_FSRS_review_unix_time); 
  stmt_column_text(stmt,  7, ARRAY_SIZE(c->last_seen), c->last_seen); 
  stmt_column_int(stmt,  8, &c->steps_until_learned); 
  stmt_column_int(stmt,  9, &c->times_seen); 
  sqlite3_finalize(stmt);

  return 0;
}


void insert_symbol_training_history(int JapanChar_id, int failed_attempts, int spent_time_ms) {
  const char *sql =
    " INSERT INTO SymbolTrainingFullHistory (JapanChar_id, failed_attempts, spent_time_ms)"
    " VALUES (?, ?, ?);"
  ;
  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_int(stmt, 1, JapanChar_id);
  sql_bind_int(stmt, 2, failed_attempts);
  sql_bind_int(stmt, 3, spent_time_ms);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void insert_card_training_FSRS_history(Card *card, FSRS_GRADE grade, int failed_symbols, int used_hint, int spent_time_ms, double FSRS_Stability_BEFORE, double FSRS_Difficulty_BEFORE, int grade_is_forced) {
  const char *sql =
    " INSERT INTO CardsTrainingFSRSHistory (Card_id, FSRS_Stability_AFTER, FSRS_Difficulty_AFTER, FSRS_Grade, used_hint, failed_symbols, spent_time_ms, FSRS_Stability_BEFORE, FSRS_Difficulty_BEFORE, grade_is_forced)"
    " VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_int(stmt, 1, card->id);
  sql_bind_double(stmt, 2, card->FSRS_Stability);
  sql_bind_double(stmt, 3, card->FSRS_Difficulty);
  sql_bind_int(stmt, 4, grade);
  sql_bind_int(stmt, 5, used_hint);
  sql_bind_int(stmt, 6, failed_symbols);
  sql_bind_int(stmt, 7, spent_time_ms);
  sql_bind_double(stmt, 8, FSRS_Stability_BEFORE);
  sql_bind_double(stmt, 9, FSRS_Difficulty_BEFORE);
  sql_bind_int(stmt, 10, grade_is_forced);
  sqlite3_step(stmt); 
  sqlite3_finalize(stmt);
}

void update_card_FSRS_data(Card* c) {
  const char *sql =
    " UPDATE Cards"
    " SET FSRS_Stability = ?, FSRS_Difficulty = ?, has_FSRS_data = 1, last_FSRS_review_unix_time = ?"
    " WHERE id = ?;"
  ;
  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_double(stmt, 1, c->FSRS_Stability);
  sql_bind_double(stmt, 2, c->FSRS_Difficulty);
  sql_bind_int(stmt, 3, time(NULL));
  sql_bind_int(stmt, 4, c->id);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void update_card_stepsUntilLearned_data(Card* c) {
  const char *sql =
    " UPDATE Cards"
    " SET steps_until_learned = ?"
    " WHERE id = ?;"
  ;
  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_int(stmt, 1, c->steps_until_learned);
  sql_bind_int(stmt, 2, c->id);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void update_card_extra_data(Card* c) {
  const char *sql =
    " UPDATE Cards"
    " SET times_seen = ?, last_seen = datetime('now', 'localtime')"
    " WHERE id = ?;"
  ;
  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_int(stmt, 1, c->times_seen+1);
  sql_bind_int(stmt, 2, c->id);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void add_new_card(int n, const wchar_t back[n], int m, const wchar_t front[m], int q, const char type[q]) {  
  const char *sql =
    "INSERT INTO Cards "
    "(back, front, type) "
    "VALUES (?, ?, ?);"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_wtext(stmt, 1, n, back);
  sql_bind_wtext(stmt,  2, m, front);
  sql_bind_text(stmt,  3, q, type);

  stmt_run_and_finish(stmt);
}

int find_card_by_wstring(Card* c, int n, const wchar_t wstring[n]) {  
  const char *sql =
    " SELECT id, back, front, FSRS_Stability, FSRS_Difficulty, has_FSRS_data, last_FSRS_review_unix_time, last_seen, steps_until_learned, times_seen"
    " FROM Cards"
    " WHERE back == ? OR front == ?;"
  ;


  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_wtext(stmt, 1, n, wstring);
  sql_bind_wtext(stmt, 2, n, wstring);
  if(sqlite3_step(stmt) != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return -1;
  }
  stmt_column_int  (stmt,  0, &c->id);
  stmt_column_wtext(stmt,  1, ARRAY_SIZE(c->back), c->back);
  stmt_column_wtext(stmt,  2, ARRAY_SIZE(c->front), c->front);
  stmt_column_double(stmt,  3, &c->FSRS_Stability);
  stmt_column_double(stmt,  4, &c->FSRS_Difficulty);
  stmt_column_int(stmt,  5, &c->has_FSRS_data); 
  stmt_column_int(stmt,  6, &c->last_FSRS_review_unix_time); 
  stmt_column_text(stmt,  7, ARRAY_SIZE(c->last_seen), c->last_seen); 
  stmt_column_int(stmt,  8, &c->steps_until_learned); 
  stmt_column_int(stmt,  9, &c->times_seen); 
  sqlite3_finalize(stmt);
  return 0;
}

void update_card_back(int id, int n, const wchar_t back[n]) {
  const char *sql =
    " UPDATE Cards"
    " SET back = ?"
    " WHERE id = ?;"
  ;
  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_wtext(stmt, 1, n, back);
  sql_bind_int(stmt, 2, id);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void update_card_front(int id, int n, const wchar_t front[n]) {
  const char *sql =
    " UPDATE Cards"
    " SET front = ?"
    " WHERE id = ?;"
  ;
  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_wtext(stmt, 1, n, front);
  sql_bind_int(stmt, 2, id);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void update_card_banished_state(int id, int banished_state) {
  const char *sql =
    " UPDATE Cards"
    " SET banished = ?"
    " WHERE id = ?;"
  ;
  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_int(stmt, 1, banished_state);
  sql_bind_int(stmt, 2, id);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void insert_card_training_history(Card *card, FSRS_GRADE grade, int failed_symbols, int used_hint, int spent_time_ms, int grade_is_forced, int suggestion_type) {
  const char *sql =
    " INSERT INTO CardsTrainingHistory (Card_id, FSRS_Stability, FSRS_Difficulty, FSRS_Grade, used_hint, failed_symbols, spent_time_ms, grade_is_forced, suggestion_type)"
    " VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_int(stmt, 1, card->id);
  sql_bind_double(stmt, 2, card->FSRS_Stability);
  sql_bind_double(stmt, 3, card->FSRS_Difficulty);
  sql_bind_int(stmt, 4, grade);
  sql_bind_int(stmt, 5, used_hint);
  sql_bind_int(stmt, 6, failed_symbols);
  sql_bind_int(stmt, 7, spent_time_ms);
  sql_bind_int(stmt, 8, grade_is_forced);
  sql_bind_int(stmt, 9, suggestion_type);
  sqlite3_step(stmt); 
  sqlite3_finalize(stmt);
}
