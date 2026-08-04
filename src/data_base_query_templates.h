#pragma once
#include<sqlite3.h>

void sql_run(const char *sql);
void stmt_run_and_finish(sqlite3_stmt *stmt);
sqlite3_stmt* sql_prepare(const char *sql);
void sql_bind_text(sqlite3_stmt *stmt, int id, const char *column);
void sql_bind_int(sqlite3_stmt *stmt, int id, int column);
