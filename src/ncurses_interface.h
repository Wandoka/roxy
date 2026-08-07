#pragma once
#include <ncurses.h>
#include "japan_char.h"

#define NCURSES_GREEN_COLOR_PAIR   1
#define NCURSES_RED_COLOR_PAIR     2


typedef struct NCursesConfig{
  int output_text_start_L;
  int output_text_start_C;
  int cursor_position_L;
  int cursor_position_C;
} NCursesConfig;

void set_default_ncurses_config(NCursesConfig *p);
void ncurses_initialize_session(NCursesConfig *new_config);
void ncurses_finish_session();
void ncurses_output_jstring(int n, JapanChar jstring[n]);
wchar_t ncurses_get_user_input_wchar();
