
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#define ROXY_GREEN_COLOR_PAIR   1
#define ROXY_RED_COLOR_PAIR     2

wchar_t hiragana_short[] = {
  L'あ', L'い', L'う', L'え', L'お',
  L'か', L'き', L'く', L'け', L'こ'
};

wchar_t get_user_input_wchar() {
  move(LINES-1,COLS-1);
  wint_t ch;
  get_wch(&ch);
  return (wchar_t) ch;
}

int current_string_position = 0;
int size_modifier = 2; //в японском символы в 2 раза шире чем в английском или русском
int corrent_symbols = 0;
wchar_t wstr[6];
void print_random_string() {
  for(int i = 0; i < 5; ++i) {
    while(1) {
      wstr[i] = hiragana_short[rand()%10];
      if(i == 0 || wstr[i-1] != wstr[i]) break;
    }
  }
  wstr[5] = 0;
  mvaddwstr(1, 1, wstr);
  move(LINES-1,COLS-1);
  refresh();
  
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
    color_current_symbol(ROXY_GREEN_COLOR_PAIR);
    ++current_string_position;
  }
  else {
    color_current_symbol(ROXY_RED_COLOR_PAIR);
  } 
  if (current_string_position == 5) {
    print_random_string();
  }
}

int run_trainer(void) {
  setlocale(LC_ALL, ""); // UTF-8
  srand(time(NULL));
                        
  initscr();                  // start ncurses mode
  cbreak();                   // disable line buffering
  noecho();                   // don't echo typed characters
  start_color();              // позволяет работать с цветами
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  curs_set(1);                
  move(LINES-1,COLS-1);
  refresh(); 
 
  print_random_string();
  while(1) {
    wchar_t c = get_user_input_wchar();
    check_and_update(c);
  }
  endwin();                   // restore terminal
  return 0;
}
