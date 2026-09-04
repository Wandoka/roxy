#include "cards_trainer.h"
#include "ncurses_interface.h"
#include "jstring_progress.h"
#include "card.h"
#include "data_base_interface.h"
#include "common.h"
#include "card_rememberence_grading.h"
#include "src/fsrs_algorithm.h"
#include "src/logger.h"
#include <stdlib.h>
#include <sys/time.h>


typedef enum {
  CURRENT_CARD_LONG_TERM_TRAINING,
  CURRENT_CARD_NEW_CARD,
  CURRENT_CARD_RANDOM,
} CURRENT_CARD_TYPE;

static Card current_card;
static CURRENT_CARD_TYPE current_card_type;
static double FSRS_Stability_BEFORE;
static double FSRS_Difficulty_BEFORE;
static int should_force_grade;
static FSRS_GRADE forced_grade;
int position_in_banned_card_ids_array = 0;
static int banned_card_ids[7] = {-1, -1, -1, -1, -1, -1, -1};

static int calc_interval_ms(struct timeval *start_time, struct timeval *end_time) {
  return (end_time->tv_sec-start_time->tv_sec)*1000+(end_time->tv_usec-start_time->tv_usec)/1000;
}

static void print_forced_grade() {
  wchar_t forced_grade_string[128];
  if(forced_grade == FSRS_RECALL_HARD) { 
    swprintf(forced_grade_string, ARRAY_SIZE(forced_grade_string), L"FORCED RECALL_HARD GRADE");
    ncurses_info_pannel2_output_wstring(ARRAY_SIZE(forced_grade_string), forced_grade_string, NCURSES_RED_COLOR_PAIR);
  }
  if(forced_grade == FSRS_RECALL_GOOD) { 
    swprintf(forced_grade_string, ARRAY_SIZE(forced_grade_string), L"FORCED RECALL_GOOD GRADE");
    ncurses_info_pannel2_output_wstring(ARRAY_SIZE(forced_grade_string), forced_grade_string, NCURSES_YELLOW_COLOR_PAIR);
  }
  if(forced_grade == FSRS_RECALL_EASY) { 
    swprintf(forced_grade_string, ARRAY_SIZE(forced_grade_string), L"FORCED RECALL_EASY GRADE");
    ncurses_info_pannel2_output_wstring(ARRAY_SIZE(forced_grade_string), forced_grade_string, NCURSES_GREEN_COLOR_PAIR);
  }

  
}

static void print_statistics() {
  wchar_t statistics_string[128];
  Card lowest_R_card;
  select_lowest_R_card(&lowest_R_card);

  double R_percent = fsrs_calc_R_recall_value(&lowest_R_card)*100;
  NCURSES_COLORS ncurses_color = NCURSES_GREEN_COLOR_PAIR;
  if(R_percent <= 90) {
    ncurses_color = NCURSES_RED_COLOR_PAIR;
  }
  swprintf(statistics_string, ARRAY_SIZE(statistics_string), L"Long term Retention = %f%%", R_percent);
  ncurses_info_pannel3_output_wstring(ARRAY_SIZE(statistics_string), statistics_string, ncurses_color);

  ncurses_color = NCURSES_GREEN_COLOR_PAIR;
  int amount_of_new_cards = count_new_cards();
  if(amount_of_new_cards != 0) {
    ncurses_color = NCURSES_RED_COLOR_PAIR;
  }
  swprintf(statistics_string, ARRAY_SIZE(statistics_string), L"Remaining new cards = %d", amount_of_new_cards);
  ncurses_info_pannel4_output_wstring(ARRAY_SIZE(statistics_string), statistics_string, ncurses_color);
}
static void set_card() {
  if(!select_lowest_R_card(&current_card) && fsrs_calc_R_recall_value(&current_card) <= 0.90) {
    current_card_type = CURRENT_CARD_LONG_TERM_TRAINING;
    FSRS_Stability_BEFORE  = current_card.FSRS_Stability;
    FSRS_Difficulty_BEFORE = current_card.FSRS_Difficulty;
  }
  else if(!select_random_new_card(&current_card, ARRAY_SIZE(banned_card_ids), banned_card_ids)) {
    current_card_type = CURRENT_CARD_NEW_CARD;
  }
  else if(!select_random_learned_card(&current_card, ARRAY_SIZE(banned_card_ids), banned_card_ids)) {
    current_card_type = CURRENT_CARD_RANDOM;
  }
  else {
    printf("NOT ENOUGH CARDS IN THE DECK TO RUN TRAINER\n");
    exit(1);
  }
  LOG_INT(current_card_type);
  LOG_INT(current_card.id);
  LOG_W(current_card.back);
  LOG_W(current_card.front);
  banned_card_ids[position_in_banned_card_ids_array++] = current_card.id;
  if(position_in_banned_card_ids_array == ARRAY_SIZE(banned_card_ids)) {
    position_in_banned_card_ids_array = 0;
  }
  LOG("----");
  set_new_hidden_string_to_Word_Progress(ARRAY_SIZE(current_card.back), current_card.back, ARRAY_SIZE(current_card.front), current_card.front);
  print_statistics();
}

static void review_card(FSRS_GRADE grade, int failed_symbols, int used_hint, int spent_time_ms) {
  if(current_card_type == CURRENT_CARD_LONG_TERM_TRAINING) {
    fsrs_review(&current_card, grade);
    update_card_FSRS_data(&current_card);
    insert_card_training_FSRS_history(&current_card, grade, failed_symbols, used_hint, spent_time_ms, FSRS_Stability_BEFORE, FSRS_Difficulty_BEFORE, should_force_grade);
  }
  else if (current_card_type == CURRENT_CARD_NEW_CARD) {
    if(grade == FSRS_RECALL_EASY) { 
      current_card.steps_until_learned--;
    }
    else if(grade == FSRS_RECALL_HARD) {
      current_card.steps_until_learned++;
      if(current_card.steps_until_learned > 2){
        current_card.steps_until_learned = 2;
      }
    }
    else if(grade == FSRS_FORGET) {
      current_card.steps_until_learned = 2;
    }
    update_card_stepsUntilLearned_data(&current_card);
    if(current_card.steps_until_learned == 0) {
      grade = FSRS_RECALL_HARD;
      should_force_grade = 1;
      fsrs_review(&current_card, FSRS_RECALL_HARD);
      update_card_FSRS_data(&current_card);
      insert_card_training_FSRS_history(&current_card, grade, failed_symbols, used_hint, spent_time_ms, FSRS_Stability_BEFORE, FSRS_Difficulty_BEFORE, should_force_grade);
    }
  }
  else if (current_card_type == CURRENT_CARD_RANDOM) {
    //do abolutely nothing
  }
  //DOING THESE EVERY TIME
  update_card_extra_data(&current_card);
  insert_card_training_history(&current_card, grade, failed_symbols, used_hint, spent_time_ms, should_force_grade, current_card_type);
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
  int used_hint = 0;
  should_force_grade = 0;
  while(1) {
    wchar_t c = ncurses_get_user_input_wchar();
    if (c == KEY_F(1)) { 
      ncurses_clear_pannel2_output_line();
      show_word_of_Word_Progress();
      used_hint = 1;
    }
    if (c == KEY_F(12)) { 
      if(used_hint == 0) {
        if(should_force_grade == 0) { 
          should_force_grade = 1;
          forced_grade = FSRS_RECALL_HARD;
        }
        else if(forced_grade == FSRS_RECALL_HARD) {
          forced_grade = FSRS_RECALL_GOOD;
        }
        else if(forced_grade == FSRS_RECALL_GOOD) {
          forced_grade = FSRS_RECALL_EASY;
        }
        else if(forced_grade == FSRS_RECALL_EASY) {
          forced_grade = FSRS_RECALL_HARD;
        }
        print_forced_grade();
      }
    }
    int good, finished;
    Word_Progress_string_by_char(c, &good, &finished);
    if(finished && good) {
      gettimeofday(&end_time,0);
      int spent_time_ms = calc_interval_ms(&start_time, &end_time);
      FSRS_GRADE grade = grade_card_rememberence(failed_symbols, used_hint, spent_time_ms);
      if(should_force_grade == 1) {
        grade = forced_grade; 
      }
      review_card(grade, failed_symbols, used_hint, spent_time_ms);

      set_card(); 
      gettimeofday(&start_time,0);
      used_hint = 0;
      failed_symbols = 0;
      should_force_grade = 0;
    }
    if(good == 0) {
      failed_symbols++;
    }
    
  }
  ncurses_finish_session();
  return;
}
