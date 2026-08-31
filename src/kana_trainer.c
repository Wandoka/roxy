#include "kana_trainer.h"
#include <wchar.h>
#include "random_generator_interface.h"
#include "ncurses_interface.h"
#include "common.h"
#include "data_base_interface.h"
#include "jstring_progress.h"
#include "src/logger.h"
#include <sys/time.h>


static int pack_size=0;
static JapanChar trainingPack[1024];
static JapanChar randomJString[6];


static void new_random_jstring() {
  int japan_chars_am = random_int(5, 5);
  pseudorandom_japan_string(pack_size, trainingPack, ARRAY_SIZE(randomJString), randomJString, japan_chars_am);  
  set_new_string_to_Symbol_Progress(japan_chars_am, randomJString);
}

static int calc_interval_ms(struct timeval *start_time, struct timeval *end_time) {
    return (end_time->tv_sec-start_time->tv_sec)*1000+(end_time->tv_usec-start_time->tv_usec)/1000;
}

int run_kana_trainer(int up_row, int down_row) {
  ncurses_initialize_session();  
  NCursesConfig config;
  get_default_ncurses_config(&config);
  ncurses_set_config(&config);
  
  select_hiragana_rows(ARRAY_SIZE(trainingPack), trainingPack, &pack_size, up_row, down_row);

  new_random_jstring();

  int current_jpos = 0;
  struct timeval start_time;
  struct timeval end_time;
  gettimeofday(&start_time,0);

  int failed_attempts = 0;
  int JapanChar_id = randomJString[0].id;
  while(1) {
    wchar_t c = ncurses_get_user_input_wchar();
    int good, finished, jpos;
    Symbol_Progress_string_by_char(c, &good, &finished, &jpos);
    if(jpos != current_jpos) {
      // If we are here - it means it is time to put logs 
      gettimeofday(&end_time,0);
      insert_symbol_training_history(JapanChar_id, failed_attempts, calc_interval_ms(&start_time, &end_time)); 

      gettimeofday(&start_time,0);
      current_jpos = jpos;
      failed_attempts = 0;
      JapanChar_id = randomJString[jpos].id;
    }
    if(good == 0) {
      failed_attempts++;
    }
    
    LOG_W(randomJString[jpos].symbol);
    LOG("\n");

    if(finished) {
      new_random_jstring();
    }
  }
  ncurses_finish_session();
  return 0;
}
