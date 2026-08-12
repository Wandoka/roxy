#include "cards_trainer.h"
#include "ncurses_interface.h"
#include "jstring_progress.h"
#include "card.h"
#include "data_base_interface.h"
#include "common.h"
#include "random_generator_interface.h"

static Card listOfCards[1024] = {0};
static int total_cards = 0;


void set_random_card() {
  Card *card = &listOfCards[random_int(0, total_cards-1)];
  set_new_hidden_string_to_progress(ARRAY_SIZE(card->japanese), card->japanese, ARRAY_SIZE(card->meaning), card->meaning);
}
void run_card_trainer() {
  ncurses_initialize_session();  
  NCursesConfig config;
  get_default_ncurses_config(&config);
  ncurses_set_config(&config);
  select_card_rows(ARRAY_SIZE(listOfCards), listOfCards, &total_cards);  

  set_random_card();
  while(1) {
    wchar_t c = ncurses_get_user_input_wchar();
    int good, finished;
    progress_string_by_char(c, &good, &finished);
    if(finished) {
      set_random_card();
    }
  }
  ncurses_finish_session();
  return;
}
