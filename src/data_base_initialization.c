#include "data_base_initialization.h"
#include "full_hiragana_table.h"
#include "data_base_query_templates.h"

sqlite3 *db;

void initialize_database() {
  sqlite3_open("roxy.db", &db);
  const char *sql_create_hiragana_table =
    "CREATE TABLE IF NOT EXISTS hiragana ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "symbol TEXT UNIQUE,"
    "romaji TEXT,"
    "row INTEGER,"
    "column INTEGER,"
    "type TEXT DEFAULT 'normal' CHECK(type IN ('normal','dakuten','handakuten','yoon','sokuon')),"
    "can_sokuon INTEGER);"
  ;
  sql_run(sql_create_hiragana_table);
  fill_full_hiragana_table();
}

