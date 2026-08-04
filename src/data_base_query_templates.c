#include "data_base_initialization.h"
#include <stdlib.h>

char *error_message = 0;
void sql_run(const char *sql) {
  int result = sqlite3_exec(db, sql, 0, 0, &error_message);
  if (result != SQLITE_OK) {
    exit(1);
  }
}

void stmt_run_and_finish(sqlite3_stmt *stmt) {
  int result = sqlite3_step(stmt);
  if (result != SQLITE_OK) {
    exit(1);
  }
  sqlite3_finalize(stmt);
}

sqlite3_stmt* sql_prepare(const char *sql) {
  sqlite3_stmt *stmt = NULL;
  int result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (result != SQLITE_OK) {
      exit(1);
  }
  return stmt;
}

void sql_bind_text(sqlite3_stmt *stmt, int id, const char *column) {
  int result = sqlite3_bind_text(stmt, id, column, -1, SQLITE_TRANSIENT);
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



