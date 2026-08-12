#include "data_base_interface.h"
#include "data_base_query_templates.h"
#include "common.h"

void select_hiragana_rows(int n, Hiragana listOfHiragana[n], int *found_rows, int up_row, int down_row) {
  const char *sql =
    "SELECT symbol, romaji, row, column, type, can_sokuon, can_yoon "
    "FROM hiragana WHERE (row >= ? AND row <= ?);"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_int(stmt, 1, up_row);
  sql_bind_int(stmt, 2, down_row);
  *found_rows = 0;
  for(int i = 0; i < n; ++i) {
    if(sqlite3_step(stmt) != SQLITE_ROW) break;
    Hiragana *h = &listOfHiragana[i];
    stmt_column_wtext(stmt, 0, ARRAY_SIZE(h->symbol), h->symbol);
    stmt_column_text(stmt,  1, ARRAY_SIZE(h->romaji), h->romaji);
    stmt_column_int(stmt,   2, &h->row);
    stmt_column_int(stmt,   3, &h->column);
    stmt_column_text(stmt,  4, ARRAY_SIZE(h->type), h->type);
    stmt_column_int(stmt,   5, &h->can_sokuon);
    stmt_column_int(stmt,   6, &h->can_yoon);
    ++*found_rows;
  }
}

void select_card_rows(int n, Card listOfCards[n], int *found_rows) {
  const char *sql =
    "SELECT japanese, meaning, addition_date "
    "FROM cards;"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  *found_rows = 0;
  for(int i = 0; i < n; ++i) {
    if(sqlite3_step(stmt) != SQLITE_ROW) break;
    Card *c = &listOfCards[i];
    stmt_column_wtext(stmt,  0, ARRAY_SIZE(c->japanese), c->japanese);
    stmt_column_wtext(stmt,  1, ARRAY_SIZE(c->meaning), c->meaning);
    stmt_column_text(stmt,   2, ARRAY_SIZE(c->addition_date), c->addition_date);
    ++*found_rows;
  }
}
