#include "data_base_initialization.h"
#include "full_hiragana_table.h"
#include "data_base_query_templates.h"
#include "adding_cards.h"

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
    "can_sokuon INTEGER,"
    "can_yoon INTEGER);"
  ;
  sql_run(sql_create_hiragana_table);
  fill_full_hiragana_table();

  const char *sql_create_cards_table =
    "CREATE TABLE IF NOT EXISTS cards ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "japanese TEXT UNIQUE,"
    "meaning TEXT,"
    "english TEXT,"
    "russian TEXT,"
    "addition_date TEXT,"
    "type TEXT DEFAULT 'phrase' CHECK(type IN ('grammar', 'word', 'phrase'))"
    ");"
  ;
  sql_run(sql_create_cards_table);
  add_all_cards();

}

