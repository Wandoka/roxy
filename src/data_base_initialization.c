#include "data_base_initialization.h"
#include "data_base_query_templates.h"

sqlite3 *db;

void initialize_database() {
  sqlite3_open("roxy.db", &db);

  const char *sql_create_hiragana_table =
    "CREATE TABLE IF NOT EXISTS hiragana ("
    "symbol TEXT PRIMARY KEY,"
    "english_transcription TEXT,"
    "row INTEGER,"
    "column INTEGER);"
  ;
  sql_run(sql_create_hiragana_table);
}

