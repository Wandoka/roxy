#include "data_base_initialization.h"
#include "full_hiragana_table.h"
#include "data_base_query_templates.h"
#include "adding_cards.h"

sqlite3 *db;

void initialize_database() {
  sqlite3_open("roxy.db", &db);
  const char *sql_create_JapanChars_table =
    " CREATE TABLE IF NOT EXISTS JapanChars ("
    " id INTEGER PRIMARY KEY AUTOINCREMENT,"
    " symbol TEXT UNIQUE,"
    " romaji TEXT,"
    " row INTEGER,"
    " column INTEGER,"
    " type TEXT CHECK(type IN ('hiragana','katakana','kanji')),"
    " subtype TEXT DEFAULT 'normal' CHECK(subtype IN ('normal','dakuten','handakuten','small')),"
    " can_sokuon INTEGER,"
    " can_yoon INTEGER);"
  ;
  sql_run(sql_create_JapanChars_table);
  fill_full_hiragana_table();

  const char *sql_create_Cards_table =
    " CREATE TABLE IF NOT EXISTS Cards ("
    " id INTEGER PRIMARY KEY AUTOINCREMENT,"
    " japanese TEXT UNIQUE,"
    " meaning TEXT,"
    " english TEXT,"
    " russian TEXT,"
    " addition_date TEXT,"
    " type TEXT DEFAULT 'phrase' CHECK(type IN ('grammar', 'word', 'phrase'))"
    " );"
  ;
  sql_run(sql_create_Cards_table);
  add_all_cards();

  const char *sql_create_SymbolTrainingStatistics =
    " CREATE TABLE IF NOT EXISTS SymbolTrainingStatistics ("
    " id INTEGER PRIMARY KEY AUTOINCREMENT,"
    " JapanChar_id INTEGER NOT NULL,"
    " failed_attempts INTEGER,"
    " spent_time_ms INTEGER NOT NULL,"
    " attempt_date TEXT DEFAULT (datetime('now', 'localtime')),"
    " FOREIGN KEY (JapanChar_id) REFERENCES JapanChars(id) ON DELETE CASCADE"
    " );"
  ;
  sql_run(sql_create_SymbolTrainingStatistics);

}

