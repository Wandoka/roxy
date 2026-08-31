#include "cards_trainer.h"
#include "ncurses_interface.h"
#include "jstring_progress.h"
#include "card.h"
#include "data_base_interface.h"
#include "common.h"
#include "card_rememberence_grading.h"
#include "logger.h"
#include <sys/time.h>
#include <time.h>

Card current_card;

static int calc_interval_ms(struct timeval *start_time, struct timeval *end_time) {
  return (end_time->tv_sec-start_time->tv_sec)*1000+(end_time->tv_usec-start_time->tv_usec)/1000;
}
void print_current_retention() {
  double elapsed_days_sience_last_review = (time(NULL) - current_card.last_review_unix_time)/(60.0*60.0*24.0);
  LOG_DOUBLE(elapsed_days_sience_last_review);
  LOG_INT(time(NULL));
  LOG_INT(current_card.last_review_unix_time);
  wchar_t retention[128];
  double R_percent = fsrs_calc_R_recall_value(elapsed_days_sience_last_review,current_card.FSRS_Stability)*100;
  LOG_DOUBLE(R_percent);
  LOG_DOUBLE(current_card.FSRS_Stability);
  swprintf(retention, ARRAY_SIZE(retention), L"Retention = %f%%", R_percent);
  ncurses_info_pannel3_output_wstring(ARRAY_SIZE(retention), retention);
}
void set_card() {
  select_lowest_R_card(&current_card);
  set_new_hidden_string_to_Word_Progress(ARRAY_SIZE(current_card.back), current_card.back, ARRAY_SIZE(current_card.front), current_card.front);
  print_current_retention();
}
void run_card_trainer() {
  ncurses_initialize_session();  
  NCursesConfig config;
  get_default_ncurses_config(&config);
  ncurses_set_config(&config);

  set_card();

  struct timeval start_time;
  struct timeval end_time;
  gettimeofday(&start_time,0);

  int failed_symbols = 0;
  int without_hint = 1;
  while(1) {
    wchar_t c = ncurses_get_user_input_wchar();
    if (c == KEY_F(1)) { 
      show_word_of_Word_Progress();
      without_hint = 0;
    }
    if (c == KEY_F(12)) { 
      //here I can edit current card
    }
    int good, finished;
    Word_Progress_string_by_char(c, &good, &finished);
    if(finished && good) {
      gettimeofday(&end_time,0);
      int spent_time_ms = calc_interval_ms(&start_time, &end_time);
      FSRS_GRADE grade = grade_card_rememberence(failed_symbols, without_hint, spent_time_ms);
      fsrs_review(&current_card, grade);
      update_card_FSRS_data(&current_card);
      insert_card_training_history(&current_card, grade, failed_symbols, without_hint, spent_time_ms);

      set_card(); 
      gettimeofday(&start_time,0);
      without_hint = 1;
      failed_symbols = 0;
    }
    if(good == 0) {
      failed_symbols++;
    }
    
  }
  ncurses_finish_session();
  return;
}
