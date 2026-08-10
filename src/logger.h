#pragma once

#include <assert.h>
#include <wchar.h>

void initialize_logger();

void _LOG(const char *file, int line, const char *name, const char *str);
void _LOG_INT(const char *file, int line, const char *name, int val);
void _LOG_W(const char *file, int line, const char *name, const wchar_t *str);

#define LOG(str)     _LOG(__FILE__, __LINE__, #str, str)
#define LOG_INT(val) _LOG_INT(__FILE__, __LINE__, #val, val)
#define LOG_W(str)   _LOG_W(__FILE__, __LINE__, #str, str)
