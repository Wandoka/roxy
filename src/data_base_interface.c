#include "full_hiragana_table.h"
#include "data_base_query_templates.h"
#include "common.h"

void select_hiragana_rows(int n, Hiragana listOfHiragana[n], int *found_rows, int up_row, int down_row) {
  const char *sql =
    "SELECT symbol, romaji, row, column, type, can_sokuon "
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
    ++*found_rows;
  }
}
