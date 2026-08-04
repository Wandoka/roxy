#include "data_base_query_templates.h"

void add_hiragana_symbol(const char* symbol, const char* english_transcription, int row, int column) {
  const char *sql =
    "INSERT INTO hiragana" 
    "(symbol, english_transcription, row, column) VALUES (?, ?, ?, ?);"
  ;
  sqlite3_stmt* stmt = sql_prepare(sql);  
  sql_bind_text(stmt, 1, symbol);
  sql_bind_text(stmt, 2, english_transcription);
  sql_bind_int(stmt, 3, row);
  sql_bind_int(stmt, 4, column);
  stmt_run_and_finish(stmt);
}
