#include "data_base_interface.h"
#include "data_base_query_templates.h"
#include "common.h"
#include <time.h>

void select_hiragana_rows(int n, JapanChar japanString[n], int *found_rows, int up_row, int down_row) {
  const char *sql =
    " SELECT id, symbol, row, column, type, subtype, can_sokuon, can_yoon"
    " FROM JapanChars WHERE type == 'hiragana' and (row >= ? AND row <= ?);"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_int(stmt, 1, up_row);
  sql_bind_int(stmt, 2, down_row);
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
}

void select_lowest_R_card(Card *c) {
  const char *sql =
    " SELECT id, back, front, FSRS_Stability, FSRS_Difficulty, has_FSRS_data, last_review_unix_time,"
    "         ((? - last_review_unix_time) / ?) / FSRS_Stability as R_approximation_for_sorting"
    " FROM Cards"
    " ORDER BY (R_approximation_for_sorting IS NULL) DESC, R_approximation_for_sorting DESC;"
    " LIMIT 1;"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_int(stmt, 1, time(NULL));
  sql_bind_double(stmt, 2, 60.0*60.0*24.0);
  if(sqlite3_step(stmt) != SQLITE_ROW) return;
  stmt_column_int  (stmt,  0, &c->id);
  stmt_column_wtext(stmt,  1, ARRAY_SIZE(c->back), c->back);
  stmt_column_wtext(stmt,  2, ARRAY_SIZE(c->front), c->front);
  stmt_column_double(stmt,  3, &c->FSRS_Stability);
  stmt_column_double(stmt,  4, &c->FSRS_Difficulty);
  stmt_column_int(stmt,  5, &c->has_FSRS_data); 
  stmt_column_int(stmt,  6, &c->last_review_unix_time); 
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
}

void insert_card_training_history(Card *card, FSRS_GRADE grade, int failed_symbols, int without_hint, int spent_time_ms) {
  const char *sql =
    " INSERT INTO CardsTrainingFullHistory (Card_id, FSRS_Stability, FSRS_Difficulty, FSRS_Grade, used_hint, failed_symbols, spent_time_ms)"
    " VALUES (?, ?, ?, ?, ?, ?, ?);"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_int(stmt, 1, card->id);
  sql_bind_int(stmt, 2, card->FSRS_Stability);
  sql_bind_int(stmt, 3, card->FSRS_Difficulty);
  sql_bind_int(stmt, 4, grade);
  sql_bind_int(stmt, 5, failed_symbols);
  sql_bind_int(stmt, 6, without_hint);
  sql_bind_int(stmt, 7, spent_time_ms);
  sqlite3_step(stmt); 
}

void update_card_FSRS_data(Card* c) {
  const char *sql =
    " UPDATE Cards"
    " SET FSRS_Stability = ?, FSRS_Difficulty = ?, has_FSRS_data = 1, last_review_unix_time = ?"
    " WHERE id = ?;"
  ;
  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_double(stmt, 1, c->FSRS_Stability);
  sql_bind_double(stmt, 2, c->FSRS_Difficulty);
  sql_bind_double(stmt, 3, time(NULL));
  sql_bind_int(stmt, 4, c->id);
  sqlite3_step(stmt);
}
