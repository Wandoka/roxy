//переименовать этот файл
#include "data_base_query_templates.h"
#include "data_base_initialization.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wchar_utf8_convertor.h"
#include "common.h"

static char *error_message = 0;
void sql_run(const char *sql) {
  int result = sqlite3_exec(db, sql, 0, 0, &error_message);
  if (result != SQLITE_OK) {
    printf("SQL error: %s\nSQL: %s\n", error_message ? error_message : sqlite3_errmsg(db), sql);
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

void sql_bind_text(sqlite3_stmt *stmt, int id, int n, const char column_data[n]) {
  int result = sqlite3_bind_text(stmt, id, column_data, -1, SQLITE_TRANSIENT);
  if (result != SQLITE_OK) {
      printf("SQL bind_text error: %s\n", sqlite3_errmsg(db));
      exit(1);
  }
}

void sql_bind_wtext(sqlite3_stmt *stmt, int id, int n, wchar_t const column_data[n]) {
  assert(n <= 1024);
  char utf8_column_data[1024];
  wchar_to_utf8(n, column_data, ARRAY_SIZE(utf8_column_data), utf8_column_data);
  int result = sqlite3_bind_text(stmt, id, utf8_column_data, -1, SQLITE_TRANSIENT);
  if (result != SQLITE_OK) {
      printf("SQL bind_wtext error: %s\n", sqlite3_errmsg(db));
      exit(1);
  }
}

void sql_bind_int(sqlite3_stmt *stmt, int id, int column) {
  int result = sqlite3_bind_int(stmt, id, column);
  if (result != SQLITE_OK) {
      printf("SQL bind_int error: %s\n", sqlite3_errmsg(db));
      exit(1);
  }
}

void stmt_column_text(sqlite3_stmt *stmt, int id, int n, char text[n]) {
  const char *temporary_pointer = (const char*)sqlite3_column_text(stmt, id);
  if (temporary_pointer) {
    strncpy(text, temporary_pointer, n - 1);
    text[n - 1] = '\0';          // обязательно
  }
  //temporary_pointer нельзя освобождать - за это отвечает sqlite
}
void stmt_column_wtext(sqlite3_stmt *stmt, int id, int n, wchar_t text[n]) {
  const char *temporary_pointer = (const char*)sqlite3_column_text(stmt, id);
  if (temporary_pointer) {
    utf8_to_wchar(strlen(temporary_pointer), temporary_pointer, n, text);
  } 
  //temporary_pointer нельзя освобождать - за это отвечает sqlite
}

void stmt_column_int(sqlite3_stmt *stmt, int id, int* v) {
  *v = sqlite3_column_int(stmt, id);
}



