#include "jstring_progress.h"
#include "ncurses_interface.h"
#include "common.h"
#include "logger.h"

static wchar_t wString[1024] = {0};
static int wString_length=0;
static int progress = 0;

static int string_is_hidden = 0;


void set_new_string_to_progress(int n, JapanChar jstring[n]) {
  string_is_hidden = 0;
  progress = 0;
  wstring_from_japan_chars(n, jstring, ARRAY_SIZE(wString), wString, &wString_length); 
  wString[wString_length] = '\0';
  ncurses_output_wstring(ARRAY_SIZE(wString), wString);
}

void set_new_hidden_string_to_progress(int n, wchar_t hidden[n], int m, wchar_t shown[m]) {
  string_is_hidden = 1;
  progress = 0;
  wcscpy(wString, hidden);
  wString_length = wcslen(hidden);
  ncurses_info_pannel_output_wstring(m, shown);
  ncurses_clear_output_line();
}


void progress_string_by_char(wchar_t c, int *good, int *finished) {
  if (c == wString[progress]) {
    color_jchar_in_position(progress, wString[progress], NCURSES_GREEN_COLOR_PAIR);
    ++progress;
    *good = 1;
  }
  else {
    wchar_t red_c = wString[progress];
    if(string_is_hidden) {
      red_c = c; 
    }
    color_jchar_in_position(progress, red_c, NCURSES_RED_COLOR_PAIR);
    *good = 0;
  } 
  LOG_INT(progress);
  LOG_INT(wString_length);
  if (progress == wString_length) {
    *finished = 1;
  }
  else {
    *finished = 0;
  }


}
