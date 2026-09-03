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

static int corrent_in_a_row = 0;
static int best_correct_in_a_row = 0;
static int total_good_answers = 0;
static int total_bad_answers = 0;
static void print_statistics() {
  wchar_t statistics_string[128];
  swprintf(statistics_string, ARRAY_SIZE(statistics_string), L"Corrent in a row = %d", corrent_in_a_row);
  ncurses_info_pannel3_output_wstring(ARRAY_SIZE(statistics_string), statistics_string, NCURSES_NO_COLOR);
  swprintf(statistics_string, ARRAY_SIZE(statistics_string), L"Best in a row = %d", best_correct_in_a_row);
  ncurses_info_pannel4_output_wstring(ARRAY_SIZE(statistics_string), statistics_string, NCURSES_NO_COLOR);
  swprintf(statistics_string, ARRAY_SIZE(statistics_string), L"Cleanness = %f%%", (double)total_good_answers / (total_good_answers + total_bad_answers)*100);
  ncurses_info_pannel5_output_wstring(ARRAY_SIZE(statistics_string), statistics_string, NCURSES_NO_COLOR);


}

int run_kana_trainer(KANA_TYPE kana_type, int up_row, int down_row, DAKUEN_HANDAKUEN dakuen_handakuen) {
  ncurses_initialize_session();  
  NCursesConfig config;
  get_default_ncurses_config(&config);
  ncurses_set_config(&config);
 
  select_kana_rows(ARRAY_SIZE(trainingPack), trainingPack, &pack_size, up_row, down_row, dakuen_handakuen, kana_type);
  
  new_random_jstring();

  int current_jpos = 0;
  struct timeval start_time;
  struct timeval end_time;
  gettimeofday(&start_time,0);

  int failed_attempts = 0;
  int JapanChar_id = randomJString[0].id;
  print_statistics();
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
      corrent_in_a_row = 0;
      total_bad_answers++;
    }
    else {
      total_good_answers++;
      corrent_in_a_row++;
      if(corrent_in_a_row > best_correct_in_a_row) {
        best_correct_in_a_row = corrent_in_a_row;
      }
    }
    print_statistics();   
    LOG_W(randomJString[jpos].symbol);
    LOG("\n");

    if(finished) {
      new_random_jstring();
    }
  }
  ncurses_finish_session();
  return 0;
}
