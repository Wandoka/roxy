#include "logger.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "folders_access.h"

static int initialized = 0;
FILE *log_file;

static const char *shorten_path(const char *file) {
  const char *found = NULL;
  for (const char *p = file; *p; p++) {
    int at_component = (p == file || p[-1] == '/' || p[-1] == '\\');
    if (at_component &&
        (p[0] == 's' || p[0] == 'S') &&
        p[1] == 'r' && p[2] == 'c' &&
        (p[3] == '/' || p[3] == '\\' || p[3] == '\0')) {
      found = p;
    }
  }
  return found ? found : file;
}

static void format_now(char *buf, size_t n) {
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  strftime(buf, n, "%Y-%m-%d %H:%M:%S", tm);
}

void initialize_logger() {
  assert(initialized == 0);
  initialized = 1;
  log_file = fopen(make_local_share_folder_path("roxy_log.txt"), "a");
}

void _LOG(const char *file, int line, const char *name, const char *str) {
  assert(initialized);
  char ts[32];
  format_now(ts, sizeof(ts));
  fprintf(log_file, "[%s] [%s:%d] %s = %s\n",
          ts, shorten_path(file), line, name, str);
  fflush(log_file);
}

void _LOG_INT(const char *file, int line, const char *name, int val) {
  assert(initialized);
  char ts[32];
  format_now(ts, sizeof(ts));
  fprintf(log_file, "[%s] [%s:%d] %s = %d\n",
          ts, shorten_path(file), line, name, val);
  fflush(log_file);
}

void _LOG_W(const char *file, int line, const char *name, const wchar_t *str) {
  assert(initialized);
  char ts[32];
  format_now(ts, sizeof(ts));
  fprintf(log_file, "[%s] [%s:%d] %s = %ls\n",
          ts, shorten_path(file), line, name, str);
  fflush(log_file);
}

void _LOG_DOUBLE(const char *file, int line, const char *name, double val) {
  assert(initialized);
  char ts[32];
  format_now(ts, sizeof(ts));
  fprintf(log_file, "[%s] [%s:%d] %s = %.17g\n",
          ts, shorten_path(file), line, name, val);
  fflush(log_file);
}
