#include "hiragana_trainer.h"
#include <wchar.h>
#include "random_generator_interface.h"
#include "ncurses_interface.h"
#include "common.h"
#include "data_base_interface.h"


static int pack_size=0;
static JapanChar trainingPack[1024];
static JapanChar randomJString[6];

int current_string_position = 0;
int size_modifier = 2; //в японском символы в 2 раза шире чем в английском или русском
int corrent_symbols = 0;
wchar_t wstr[6];
void print_random_string() {
  int japan_chars_am = random_int(4, 6);
  random_japan_string(ARRAY_SIZE(trainingPack), trainingPack, ARRAY_SIZE(randomJString), randomJString, japan_chars_am); 
  ncurses_output_jstring(japan_chars_am, randomJString);
  //обновляю данные для текущей строки
  current_string_position = 0;
}

void color_current_symbol(int color_pair_id) {
  attron(COLOR_PAIR(color_pair_id));
  mvaddnwstr(1, 1+current_string_position*size_modifier, wstr+current_string_position, 1); 
  move(LINES-1,COLS-1);
  attroff(COLOR_PAIR(color_pair_id));
  refresh(); 
}
void check_and_update(wchar_t c) { 
  mvaddnwstr(5, 1, &c, 1);
  move(LINES-1,COLS-1);
  if (c == wstr[current_string_position]) {
    color_current_symbol(NCURSES_GREEN_COLOR_PAIR);
    ++current_string_position;
  }
  else {
    color_current_symbol(NCURSES_RED_COLOR_PAIR);
  } 
  if (current_string_position == 5) {
    print_random_string();
  }
}

int run_trainer(int up_row, int down_row) {
  NCursesConfig config;
  set_default_ncurses_config(&config);
  ncurses_initialize_session(&config);  
  
  Hiragana listOfHiragana[1024];
  int found_rows;
  select_hiragana_rows(ARRAY_SIZE(listOfHiragana), listOfHiragana, &found_rows, up_row, down_row);
  unite_hiragana_kanji_katagana(found_rows, listOfHiragana, 0, NULL, 0, NULL, ARRAY_SIZE(trainingPack), trainingPack, &pack_size);
  print_random_string();
  while(1) {
    wchar_t c = ncurses_get_user_input_wchar();
    check_and_update(c);
  }
  ncurses_finish_session();
  return 0;
}
