//переименовать этот файл
#include "data_base_initialization.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wchar_utf8_convertor.h"

static char *error_message = 0;
void sql_run(const char *sql) {
  int result = sqlite3_exec(db, sql, 0, 0, &error_message);
  if (result != SQLITE_OK) {
    exit(1);
  }
}

void stmt_run_and_finish(sqlite3_stmt *stmt) {
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

sqlite3_stmt* sql_prepare(const char *sql) {
  sqlite3_stmt *stmt = NULL;
  int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (result != SQLITE_OK) {
      printf("SQL prepare error: %s\nSQL: %s\n", sqlite3_errmsg(db), sql);
      exit(1);
  }
  return stmt;
}

void sql_bind_text(sqlite3_stmt *stmt, int id, const char *column_data) {
  int result = sqlite3_bind_text(stmt, id, column_data, -1, SQLITE_TRANSIENT);
  if (result != SQLITE_OK) {
      exit(1);
  }
}

void sql_bind_wtext(sqlite3_stmt *stmt, int id, const wchar_t *column_data) {
  char* utf8_column_data = wchar_to_utf8(column_data);
  int result = sqlite3_bind_text(stmt, id, utf8_column_data, -1, SQLITE_TRANSIENT);
  free(utf8_column_data);
  if (result != SQLITE_OK) {
      exit(1);
  }
}


void sql_bind_int(sqlite3_stmt *stmt, int id, int column) {
  int result = sqlite3_bind_int(stmt, id, column);
  if (result != SQLITE_OK) {
      exit(1);
  }
}

void stmt_column_text(sqlite3_stmt *stmt, int id, char** text) {
  const char *temporary_pointer = (const char*)sqlite3_column_text(stmt, id);
  if (temporary_pointer) {
    *text = strdup(temporary_pointer);          // или своя функция копирования
  } 
}
void stmt_column_wtext(sqlite3_stmt *stmt, int id, wchar_t** text) {
  const char *temporary_pointer = (const char*)sqlite3_column_text(stmt, id);
  if (temporary_pointer) {
    char* new_temporary_pointer = strdup(temporary_pointer);
    *text = utf8_to_wchar(new_temporary_pointer); // мне кажется тут есть memory leak, тут я записываю память, а потом её не освобождаю
  } 
}

void stmt_column_int(sqlite3_stmt *stmt, int id, int* v) {
  const char *temporary_pointer = (const char*)sqlite3_column_text(stmt, id);
  if (temporary_pointer) {
    *v = *temporary_pointer; 
  }
}



