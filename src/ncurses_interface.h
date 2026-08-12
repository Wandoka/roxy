#pragma once
#include <ncurses.h>

#define NCURSES_GREEN_COLOR_PAIR   1
#define NCURSES_RED_COLOR_PAIR     2


typedef struct NCursesConfig{
  int output_text_start_L;
  int output_text_start_C;
  int cursor_position_L;
  int cursor_position_C;
  int input_echo_start_L;
  int input_echo_start_C;
  int output_text_info_pannel_start_L;
  int output_text_info_pannel_start_C;
} NCursesConfig;

void get_default_ncurses_config(NCursesConfig *p);
void ncurses_initialize_session();
void ncurses_set_config(NCursesConfig *new_config);
void ncurses_finish_session();
void ncurses_output_wstring(int n, wchar_t wstring[n]);
void ncurses_clear_output_line();
wchar_t ncurses_get_user_input_wchar();
void color_jchar_in_position(int position, wchar_t c, int color_pair_id);
void ncurses_info_pannel_output_wstring(int n, wchar_t wstring[n]);
