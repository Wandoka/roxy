#include "jstring_progress.h"
#include "ncurses_interface.h"
#include "common.h"
#include <assert.h>

static wchar_t wString[1024] = {0};
static int wString_length=0;
static JapanChar jString[128] = {};
static int jString_length=0;
static int progress = 0;
static int jString_position = 0;
static int remaining_wchar_in_current_jString_position = 0;

typedef enum {
    NONE = 0,
    SYMBOL_PROGRESS = 1,
    WORD_PROGRESS  = 1
} JSTRING_PROGRESS;

static JSTRING_PROGRESS type_of_progress = NONE;


void set_new_string_to_Symbol_Progress(int n, JapanChar new_jstring[n]) {
  type_of_progress = SYMBOL_PROGRESS;
  assert(n < 128);
  jString_length = n;
  for(int i = 0; i < n; ++i) {
    copy_JapanChar(&jString[i], &new_jstring[i]);
  }

  progress = 0;
  jString_position = 0;
  remaining_wchar_in_current_jString_position = wcslen(jString[jString_position].symbol);

  wstring_from_japan_chars(jString_length, jString, ARRAY_SIZE(wString), wString, &wString_length); 
  wString[wString_length] = '\0';
  ncurses_output_wstring(ARRAY_SIZE(wString), wString);
}

void Symbol_Progress_string_by_char(wchar_t c, int *good, int *finished, int *jpos) {
  assert(type_of_progress==SYMBOL_PROGRESS);
  if (c == wString[progress]) {
    color_jchar_in_position(progress, wString[progress], NCURSES_GREEN_COLOR_PAIR);
    ++progress;
    assert(remaining_wchar_in_current_jString_position>0);
    --remaining_wchar_in_current_jString_position;
    if(remaining_wchar_in_current_jString_position == 0) {
      ++jString_position; 
      if(progress != wString_length) {
        remaining_wchar_in_current_jString_position = wcslen(jString[jString_position].symbol);
      }
      *jpos = jString_position-1;
    }
    *good = 1;
  }
  else {
    color_jchar_in_position(progress, wString[progress], NCURSES_RED_COLOR_PAIR);
    *good = 0;
    *jpos = jString_position;
  } 
  if (progress == wString_length) {
    *finished = 1;
  }
  else {
    *finished = 0;
  }
}

void set_new_hidden_string_to_Word_Progress(int n, wchar_t hidden[n], int m, wchar_t shown[m]) {
  type_of_progress = WORD_PROGRESS;
  progress = 0;
  wcscpy(wString, hidden);
  wString_length = wcslen(hidden);
  ncurses_info_pannel_output_wstring(m, shown);
  ncurses_clear_output_line();
  ncurses_clear_pannel2_output_line();
}


void show_word_of_Word_Progress() {
  ncurses_info_pannel2_output_wstring(ARRAY_SIZE(wString), wString);
}

void Word_Progress_string_by_char(wchar_t c, int *good, int *finished) {
  assert(type_of_progress==WORD_PROGRESS);
  if (c == wString[progress]) {
    color_jchar_in_position(progress, wString[progress], NCURSES_GREEN_COLOR_PAIR);
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

