#pragma once
#include<sqlite3.h>
#include <wchar.h>

void sql_run(const char *sql);
void stmt_run_and_finish(sqlite3_stmt *stmt);
sqlite3_stmt* sql_prepare(const char *sql);
void sql_bind_text(sqlite3_stmt *stmt, int id, int n, char column_data[n]);
void sql_bind_wtext(sqlite3_stmt *stmt, int id, int n, wchar_t column_data[n]);
void sql_bind_int(sqlite3_stmt *stmt, int id, int column);

void stmt_column_text(sqlite3_stmt *stmt, int id, int n, char text[n]);
void stmt_column_wtext(sqlite3_stmt *stmt, int id, int n, wchar_t text[n]);
void stmt_column_int(sqlite3_stmt *stmt, int id, int* v);
