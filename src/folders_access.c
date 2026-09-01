#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

static int mkdir_p(const char *path) {
  char tmp[512];
  snprintf(tmp, sizeof tmp, "%s", path);
  for (char *p = tmp + 1; *p; p++) {
    if (*p == '/') {
      *p = 0;
      if (mkdir(tmp, 0700) && errno != EEXIST) {
        return -1;
      }
      *p = '/';
    }
  }
  return (mkdir(tmp, 0700) && errno != EEXIST) ? -1 : 0;
}

static const char *roxy_path(char *buf, size_t n, const char *base, const char *file_name) {
  const char *home = getenv("HOME");
  if (!home || !file_name) return NULL;

  snprintf(buf, n, "%s/%s/roxy", home, base);
  if (mkdir_p(buf)) return NULL;

  snprintf(buf, n, "%s/%s/roxy/%s", home, base, file_name);
  return buf;
}

const char *make_local_share_folder_path(const char *file_name)
{
  static char path[512];
  return roxy_path(path, sizeof path, ".local/share", file_name);
}

const char *make_config_folder_path(const char *file_name)
{
  static char path[512];
  return roxy_path(path, sizeof path, ".config", file_name);
}
