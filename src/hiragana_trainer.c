#include "hiragana_trainer.h"
#include <wchar.h>
#include "random_generator_interface.h"
#include "ncurses_interface.h"
#include "common.h"
#include "data_base_interface.h"
#include "jstring_progress.h"
#include "logger.h"


static int pack_size=0;
static JapanChar trainingPack[1024];
static JapanChar randomJString[6];


static void new_random_jstring() {
  int japan_chars_am = random_int(5, 5);
  pseudorandom_japan_string(pack_size, trainingPack, ARRAY_SIZE(randomJString), randomJString, japan_chars_am);  
  for(int i = 0; i < japan_chars_am; ++i) {
    LOG("+++++++++++++++");
    LOG_W(randomJString[i].hiragana.symbol);
    LOG(randomJString[i].hiragana.romaji);
    LOG_INT(randomJString[i].hiragana.row);
    LOG_INT(randomJString[i].hiragana.column);
    LOG("+++++++++++++++");

  }
  LOG("WANDOKA1");
  set_new_string_to_progress(japan_chars_am, randomJString);
  LOG("WANDOKA2");
}

int run_hiragana_trainer(int up_row, int down_row) {
  ncurses_initialize_session();  
  NCursesConfig config;
  get_default_ncurses_config(&config);
  ncurses_set_config(&config);
  
  Hiragana listOfHiragana[1024];
  int found_rows;
  select_hiragana_rows(ARRAY_SIZE(listOfHiragana), listOfHiragana, &found_rows, up_row, down_row);
  LOG_INT(found_rows);
  for(int i = 0; i < found_rows; ++i) {
    LOG_W(listOfHiragana[i].symbol);
    LOG(listOfHiragana[i].romaji);
    LOG_INT(listOfHiragana[i].row);
    LOG_INT(listOfHiragana[i].column);
    LOG("---------------");
  }
  unite_hiragana_kanji_katagana(found_rows, listOfHiragana, 0, NULL, 0, NULL, ARRAY_SIZE(trainingPack), trainingPack, &pack_size);
 
  new_random_jstring();

  while(1) {
    wchar_t c = ncurses_get_user_input_wchar();
    int good, finished;
    progress_string_by_char(c, &good, &finished);
    if(finished) {
      new_random_jstring();
    }
  }
  ncurses_finish_session();
  return 0;
}
