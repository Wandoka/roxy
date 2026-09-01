#include "cards_trainer.h"
#include "ncurses_interface.h"
#include "jstring_progress.h"
#include "card.h"
#include "data_base_interface.h"
#include "common.h"
#include "card_rememberence_grading.h"
#include "src/fsrs_algorithm.h"
#include "src/logger.h"
#include <sys/time.h>
#include <time.h>


typedef enum {
  CURRENT_CARD_LONG_TERM_TRAINING,
  CURRENT_CARD_NEW_CARD,
  CURRENT_CARD_RANDOM,
} CURRENT_CARD_TYPE;

static Card current_card;
static CURRENT_CARD_TYPE current_card_type;

static int calc_interval_ms(struct timeval *start_time, struct timeval *end_time) {
  return (end_time->tv_sec-start_time->tv_sec)*1000+(end_time->tv_usec-start_time->tv_usec)/1000;
}

static int is_R_below_threshhold(Card *card) {
  double elapsed_days_since_last_review = (time(NULL) - card->last_FSRS_review_unix_time)/(60.0*60.0*24.0);
  double R = fsrs_calc_R_recall_value(elapsed_days_since_last_review,card->FSRS_Stability);
  return R <= 0.90;
}
static void print_statistics() {
  wchar_t statistics_string[128];
  double elapsed_days_since_last_review = (time(NULL) - current_card.last_FSRS_review_unix_time)/(60.0*60.0*24.0);
  double R_percent = fsrs_calc_R_recall_value(elapsed_days_since_last_review,current_card.FSRS_Stability)*100;
  swprintf(statistics_string, ARRAY_SIZE(statistics_string), L"Long term Retention = %f%%", R_percent);
  ncurses_info_pannel3_output_wstring(ARRAY_SIZE(statistics_string), statistics_string);
  swprintf(statistics_string, ARRAY_SIZE(statistics_string), L"Remaining new cards = %d", count_new_cards());
  ncurses_info_pannel4_output_wstring(ARRAY_SIZE(statistics_string), statistics_string);

}
static void set_card() {
  if(!select_lowest_R_card(&current_card) && is_R_below_threshhold(&current_card)) {
    current_card_type = CURRENT_CARD_LONG_TERM_TRAINING;
  }
  else if(!select_random_new_card(&current_card)) {
    current_card_type = CURRENT_CARD_NEW_CARD;
  }
  else if(!select_random_learned_card(&current_card)) {
    current_card_type = CURRENT_CARD_RANDOM;
  }
  LOG_INT(current_card_type);
  LOG_INT(current_card.id);
  LOG_W(current_card.back);
  LOG_W(current_card.front);
  LOG("----");
  set_new_hidden_string_to_Word_Progress(ARRAY_SIZE(current_card.back), current_card.back, ARRAY_SIZE(current_card.front), current_card.front);
  print_statistics();
}

static void review_card(FSRS_GRADE grade) {
  if(current_card_type == CURRENT_CARD_LONG_TERM_TRAINING) {
    fsrs_review(&current_card, grade);
    update_card_FSRS_data(&current_card);
  }
  else if (current_card_type == CURRENT_CARD_NEW_CARD) {
    current_card.steps_until_learned--;
    update_card_stepsUntilLearned_data(&current_card);
    if(current_card.steps_until_learned == 0) {
      fsrs_review(&current_card, FSRS_RECALL_HARD);
      update_card_FSRS_data(&current_card);
    }
  }
  else if (current_card_type == CURRENT_CARD_RANDOM) {
    //do abolutely nothing
  }
  //DOING THIS ONE TIME EVERY TIME
  update_card_extra_data(&current_card);
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
      insert_card_training_history(&current_card, grade, failed_symbols, without_hint, spent_time_ms);
      review_card(grade);

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
