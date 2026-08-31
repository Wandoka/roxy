#include "logger.h"
#include <stdio.h>

static int initialized = 0;
FILE *log_file;
void initialize_logger() {
  assert(initialized == 0);
  initialized = 1;
  log_file = fopen("debug.log", "a");
}

void _LOG(const char *file, int line, const char *name, const char *str) {
    assert(initialized);
    fprintf(log_file, "[%s:%d] %s = %s\n", file, line, name, str);
    fflush(log_file);
}

void _LOG_INT(const char *file, int line, const char *name, int val) {
    assert(initialized);
    fprintf(log_file, "[%s:%d] %s = %d\n", file, line, name, val);
    fflush(log_file);
}

void _LOG_W(const char *file, int line, const char *name, const wchar_t *str) {
    assert(initialized);
    fprintf(log_file, "[%s:%d] %s = %ls\n", file, line, name, str);
    fflush(log_file);
}

void _LOG_DOUBLE(const char *file, int line, const char *name, double val) {
    assert(initialized);
    fprintf(log_file, "[%s:%d] %s = %.17g\n", file, line, name, val);
    fflush(log_file);
}
