#include "data_base_query_templates.h"
#include "full_hiragana_table.h"

void select_hiragana_rows(Hiragana *hiragana_list, int *found_rows, int list_max_size, int up_row, int down_row) {
  const char *sql =
    "SELECT symbol, romaji, row, column, type, can_sokuon "
    "FROM hiragana WHERE (row >= ? AND row <= ?);"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_int(stmt, 1, up_row);
  sql_bind_int(stmt, 2, down_row);
  *found_rows = 0;
  while (*found_rows < list_max_size && sqlite3_step(stmt) == SQLITE_ROW) {
    Hiragana *h = &hiragana_list[*found_rows];
    stmt_column_wtext(stmt, 0, &h->symbol);
    stmt_column_text(stmt,  1, &h->romaji);
    stmt_column_int(stmt,   2, &h->row);
    stmt_column_int(stmt,   3, &h->column);
    stmt_column_text(stmt,  4, &h->type);
    stmt_column_int(stmt,   5, &h->can_sokuon);
    *found_rows+=1;
  }
}
