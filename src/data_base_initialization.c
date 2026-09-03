#include "data_base_initialization.h"
#include "full_hiragana_table.h"
#include "full_katakana_table.h"
#include "data_base_query_templates.h"
#include "adding_cards.h"
#include "folders_access.h"

sqlite3 *db;

void initialize_database() {
  sqlite3_open(make_local_share_folder_path("roxy_sqlite3.db"), &db);
  const char *sql_create_JapanChars_table =
    " CREATE TABLE IF NOT EXISTS JapanChars ("
    " id INTEGER PRIMARY KEY AUTOINCREMENT,"
    " symbol TEXT UNIQUE,"
    " row INTEGER,"
    " column INTEGER,"
    " type TEXT CHECK(type IN ('hiragana','katakana','kanji')),"
    " subtype TEXT DEFAULT 'normal' CHECK(subtype IN ('normal','dakuten','handakuten','small')),"
    " can_sokuon INTEGER,"
    " can_yoon INTEGER);"
  ;
  sql_run(sql_create_JapanChars_table);
  fill_full_hiragana_table();
  fill_full_katakana_table();

  const char *sql_create_SymbolTrainingFullHistory_table =
    " CREATE TABLE IF NOT EXISTS SymbolTrainingFullHistory ("
    " id INTEGER PRIMARY KEY AUTOINCREMENT,"
    " JapanChar_id INTEGER NOT NULL,"
    " failed_attempts INTEGER,"
    " spent_time_ms INTEGER NOT NULL,"
    " attempt_date TEXT DEFAULT (datetime('now', 'localtime')),"
    " FOREIGN KEY (JapanChar_id) REFERENCES JapanChars(id) ON DELETE CASCADE"
    " );"
  ;
  sql_run(sql_create_SymbolTrainingFullHistory_table);


  const char *sql_create_Cards_table =
    " CREATE TABLE IF NOT EXISTS Cards ("
    " id INTEGER PRIMARY KEY AUTOINCREMENT,"
    " back TEXT UNIQUE,"
    " front TEXT,"
    " FSRS_Stability DOUBLE,"
    " FSRS_Difficulty DOUBLE,"
    " has_FSRS_data INTEGER DEFAULT 0,"
    " last_FSRS_review_unix_time INTEGER,"
    " when_added TEXT DEFAULT (datetime('now', 'localtime')),"
    " type TEXT DEFAULT 'phrase',"
    " last_seen TEXT,"
    " times_seen INTEGER DEFAULT 0,"
    " steps_until_learned INTEGER DEFAULT 2"
    " );"
  ;
  sql_run(sql_create_Cards_table);
  //add_all_cards();

  const char *sql_create_CardsTrainingFSRSHistory_table =
    " CREATE TABLE IF NOT EXISTS CardsTrainingFSRSHistory ("
    " id INTEGER PRIMARY KEY AUTOINCREMENT,"
    " Card_id INTEGER NOT NULL,"
    " FSRS_Stability DOUBLE,"
    " FSRS_Difficulty DOUBLE,"
    " FSRS_Grade INTEGER,"
    " used_hint INTEGER,"
    " failed_symbols INTEGER,"
    " spent_time_ms INTEGER NOT NULL,"
    " when_attempted TEXT DEFAULT (datetime('now', 'localtime')),"
    " FOREIGN KEY (Card_id) REFERENCES Cards(id) ON DELETE CASCADE"
    " );"
  ;
  sql_run(sql_create_CardsTrainingFSRSHistory_table);
}

