#include <assert.h>
#include <ncurses.h>
#include <wchar.h>
#include "ncurses_interface.h"
#include "common.h"

static const int JAPANESE_SIZE = 2; //в японском символы в 2 раза шире чем в английском или русском
static int ncurses_initialized = 0;

void get_default_ncurses_config(NCursesConfig *p) {
  assert(ncurses_initialized);
  p->output_text_start_L = 1;
  p->output_text_start_C = 1;

  p->cursor_position_L = LINES-1;
  p->cursor_position_C = COLS-1;

  p->input_echo_start_L = 8;
  p->input_echo_start_C = 1;

  p->output_text_info_pannel_start_L = 3;
  p->output_text_info_pannel_start_C = 1;

  p->output_text_info_pannel2_start_L = 4;
  p->output_text_info_pannel2_start_C = 1;

  p->output_text_info_pannel3_start_L = 5;
  p->output_text_info_pannel3_start_C = 1;

  p->output_text_info_pannel4_start_L = 6;
  p->output_text_info_pannel4_start_C = 1;

  p->output_text_info_pannel5_start_L = 7;
  p->output_text_info_pannel5_start_C = 1;

  return;
}

static NCursesConfig config;

static void set_cursor_to_default_position() {
  move(config.cursor_position_L,config.cursor_position_C);
}

void ncurses_initialize_session() {
  if(ncurses_initialized) return;
  ncurses_initialized=1;
  initscr(); // start ncurses mode
  cbreak();  // disable line buffering
  noecho();  // don't echo typed characters
  start_color(); // позволяет работать с цветами
  keypad(stdscr, TRUE);
  init_pair(NCURSES_GREEN_COLOR_PAIR, COLOR_GREEN, COLOR_BLACK);
  init_pair(NCURSES_RED_COLOR_PAIR, COLOR_RED, COLOR_BLACK);
  init_pair(NCURSES_YELLOW_COLOR_PAIR, COLOR_YELLOW, COLOR_BLACK);
  curs_set(1);                
  set_cursor_to_default_position();
  refresh(); 
}

void ncurses_set_config(NCursesConfig *new_config) {
  assert(ncurses_initialized);
  config = *new_config;
}

void ncurses_finish_session() {
  if(!ncurses_initialized) return;
  ncurses_initialized = 0;
  endwin(); // restore terminal
}


void ncurses_output_wstring(int n, wchar_t wstring[n]) {
  mvaddwstr(config.output_text_start_L, config.output_text_start_C, wstring);
  clrtoeol(); //удаляем всё, что осталось на строчке дальше справа
  set_cursor_to_default_position();
  refresh();
}


void ncurses_info_pannel_output_wstring(int n, wchar_t wstring[n]) {
  mvaddwstr(config.output_text_info_pannel_start_L, config.output_text_info_pannel_start_C, wstring);
  clrtoeol(); //удаляем всё, что осталось на строчке дальше справа
  set_cursor_to_default_position();
  refresh();
}
void ncurses_info_pannel2_output_wstring(int n, wchar_t wstring[n], NCURSES_COLORS color_pair_id) {
  if(color_pair_id != NCURSES_NO_COLOR) attron(COLOR_PAIR(color_pair_id));
  mvaddwstr(config.output_text_info_pannel2_start_L, config.output_text_info_pannel2_start_C, wstring);
  clrtoeol(); //удаляем всё, что осталось на строчке дальше справа
  if(color_pair_id != NCURSES_NO_COLOR) attron(COLOR_PAIR(color_pair_id));
  set_cursor_to_default_position();
  refresh();
}
void ncurses_info_pannel3_output_wstring(int n, wchar_t wstring[n], NCURSES_COLORS color_pair_id) {
  if(color_pair_id != NCURSES_NO_COLOR) attron(COLOR_PAIR(color_pair_id));
  mvaddwstr(config.output_text_info_pannel3_start_L, config.output_text_info_pannel3_start_C, wstring);
  clrtoeol(); //удаляем всё, что осталось на строчке дальше справа
  if(color_pair_id != NCURSES_NO_COLOR) attroff(COLOR_PAIR(color_pair_id));
  set_cursor_to_default_position();
  refresh();
}
void ncurses_info_pannel4_output_wstring(int n, wchar_t wstring[n], NCURSES_COLORS color_pair_id) {
  if(color_pair_id != NCURSES_NO_COLOR) attron(COLOR_PAIR(color_pair_id));
  mvaddwstr(config.output_text_info_pannel4_start_L, config.output_text_info_pannel4_start_C, wstring);
  clrtoeol(); //удаляем всё, что осталось на строчке дальше справа
  attroff(COLOR_PAIR(color_pair_id));
  if(color_pair_id != NCURSES_NO_COLOR) set_cursor_to_default_position();
  refresh();
}

void ncurses_info_pannel5_output_wstring(int n, wchar_t wstring[n], NCURSES_COLORS color_pair_id) {
  if(color_pair_id != NCURSES_NO_COLOR) attron(COLOR_PAIR(color_pair_id));
  mvaddwstr(config.output_text_info_pannel5_start_L, config.output_text_info_pannel5_start_C, wstring);
  clrtoeol(); //удаляем всё, что осталось на строчке дальше справа
  if(color_pair_id != NCURSES_NO_COLOR) attroff(COLOR_PAIR(color_pair_id));
  set_cursor_to_default_position();
  refresh();
}




void ncurses_clear_output_line() {
  move(config.output_text_start_L, config.output_text_start_C);
  clrtoeol(); //удаляем всё, что осталось на строчке дальше справа
  set_cursor_to_default_position();
  refresh();
}

void ncurses_clear_pannel2_output_line() {
  move(config.output_text_info_pannel2_start_L, config.output_text_info_pannel2_start_C);
  clrtoeol(); //удаляем всё, что осталось на строчке дальше справа
  set_cursor_to_default_position();
  refresh();
}
void ncurses_clear_pannel3_output_line() {
  move(config.output_text_info_pannel3_start_L, config.output_text_info_pannel3_start_C);
  clrtoeol(); //удаляем всё, что осталось на строчке дальше справа
  set_cursor_to_default_position();
  refresh();
}
void ncurses_clear_pannel4_output_line() {
  move(config.output_text_info_pannel4_start_L, config.output_text_info_pannel4_start_C);
  clrtoeol(); //удаляем всё, что осталось на строчке дальше справа
  set_cursor_to_default_position();
  refresh();
}
void ncurses_clear_pannel5_output_line() {
  move(config.output_text_info_pannel5_start_L, config.output_text_info_pannel5_start_C);
  clrtoeol(); //удаляем всё, что осталось на строчке дальше справа
  set_cursor_to_default_position();
  refresh();
}






wchar_t ncurses_get_user_input_wchar() {
  set_cursor_to_default_position();
  wchar_t ch;
  get_wch((wint_t*)&ch);
  mvaddnwstr(config.input_echo_start_L, config.input_echo_start_C, &ch, 1);
  set_cursor_to_default_position();
  return (wchar_t) ch;
}

void color_jchar_in_position(int position, wchar_t c, NCURSES_COLORS color_pair_id) {
  if(color_pair_id != NCURSES_NO_COLOR) attron(COLOR_PAIR(color_pair_id));
  mvaddnwstr(config.output_text_start_L, config.output_text_start_C+position*JAPANESE_SIZE, &c, 1); 
  set_cursor_to_default_position();
  if(color_pair_id != NCURSES_NO_COLOR) attroff(COLOR_PAIR(color_pair_id));
  refresh(); 
}
