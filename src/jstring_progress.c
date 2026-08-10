#include "jstring_progress.h"
#include "ncurses_interface.h"
#include "common.h"
#include "logger.h"

static wchar_t wString[1024] = {0};
static int wString_length=0;
static int progress = 0;

void set_new_string_to_progress(int n, JapanChar jstring[n]) {
  progress = 0;
  wstring_from_japan_chars(n, jstring, ARRAY_SIZE(wString), wString, &wString_length); 
  wString[wString_length] = '\0';
  ncurses_output_wstring(ARRAY_SIZE(wString), wString);
  LOG_INT(n);
  for(int i = 0; i < n; ++i) {
    LOG_W(jstring[i].hiragana.symbol);
    LOG(jstring[i].hiragana.romaji);
  }
  LOG_INT(wString_length);
  LOG_W(wString);
}

void progress_string_by_char(wchar_t c, int *good, int *finished) {
  if (c == wString[progress]) {
    color_jchar_in_position(progress, c, NCURSES_GREEN_COLOR_PAIR);
    ++progress;
    *good = 1;
  }
  else {
    color_jchar_in_position(progress, c, NCURSES_RED_COLOR_PAIR);
    *good = 0;
  } 
  if (progress == wString_length) {
    *finished = 1;
  }
  else {
    *finished = 0;
  }


}
