#include "hiragana_trainer.h"
#include <wchar.h>
#include "random_generator_interface.h"
#include "ncurses_interface.h"
#include "common.h"
#include "data_base_interface.h"
#include "jstring_progress.h"


static int pack_size=0;
static JapanChar trainingPack[1024];
static JapanChar randomJString[6];


static void new_random_jstring() {
  int japan_chars_am = random_int(5, 5);
  pseudorandom_japan_string(pack_size, trainingPack, ARRAY_SIZE(randomJString), randomJString, japan_chars_am);  
  set_new_string_to_progress(japan_chars_am, randomJString);
}

int run_hiragana_trainer(int up_row, int down_row) {
  ncurses_initialize_session();  
  NCursesConfig config;
  get_default_ncurses_config(&config);
  ncurses_set_config(&config);
  
  int found_rows;
  select_hiragana_rows(ARRAY_SIZE(trainingPack), trainingPack, &found_rows, up_row, down_row);
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
