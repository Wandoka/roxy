#include <ncurses.h>
#include "ncurses_interface.h"
#include "common.h"

void set_default_ncurses_config(NCursesConfig *p) {
  p->output_text_start_L = 1;
  p->output_text_start_C = 1;

  p->cursor_position_L = LINES-1;
  p->cursor_position_C = COLS-1;
  return;
}

static NCursesConfig config;
static int ncurses_initialized = 0;

static void set_cursor_to_default_position() {
  move(config.cursor_position_L,config.cursor_position_C);
}

void ncurses_initialize_session(NCursesConfig *new_config) {
  if(ncurses_initialized) return;
  ncurses_initialized=1;
  config = *new_config;
  initscr(); // start ncurses mode
  cbreak();  // disable line buffering
  noecho();  // don't echo typed characters
  start_color(); // позволяет работать с цветами
  init_pair(NCURSES_GREEN_COLOR_PAIR, COLOR_GREEN, COLOR_BLACK);
  init_pair(NCURSES_RED_COLOR_PAIR, COLOR_RED, COLOR_BLACK);
  curs_set(1);                
  set_cursor_to_default_position();
  refresh(); 

}
void ncurses_finish_session() {
  if(!ncurses_initialized) return;
  ncurses_initialized = 0;
  endwin(); // restore terminal
}

void ncurses_output_jstring(int n, JapanChar jstring[n]) {
  wchar_t wstring[100]={0};
  int length = 0;
  wstring_from_japan_chars(n, jstring, ARRAY_SIZE(wstring), wstring, &length);
  mvaddwstr(config.output_text_start_L, config.output_text_start_C, wstring);
  set_cursor_to_default_position();
  refresh();
}

wchar_t ncurses_get_user_input_wchar() {
  set_cursor_to_default_position();
  wint_t ch;
  get_wch(&ch);
  return (wchar_t) ch;
}

wchar_t color_jchar_in_position(int position, int color_pair_id) {
  set_cursor_to_default_position();
  wint_t ch;
  get_wch(&ch);
  return (wchar_t) ch;
}
