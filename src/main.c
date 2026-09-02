#include "args_handling.h"
#include "data_base_initialization.h"
#include "common.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "random_generator_interface.h"
#include "kana_trainer.h"
#include "logger.h"
#include "cards_trainer.h"
#include "wchar_utf8_convertor.h"
#include "card_deck_manipulations.h"


static void kana_training(int argc, char *argv[argc], KANA_TYPE kana_type) {
  (void) argc; (void) argv; 
  DAKUEN_HANDAKUEN dakuen_handakuen = NO_DAKUEN_HANDAKUEN;
  int up_row   = 0;
  int down_row = 10;
  if(argc >= 1 && !strcmp(argv[0], "+")) {
    dakuen_handakuen = YES_DAKUEN_HANDAKUEN;
  }
  if(argc >= 2) {
    up_row = atoi(argv[1]);
    if(up_row < 0) up_row = 0;
    if(up_row > 10) up_row = 10;
  }
  if(argc >= 3) {
    down_row = atoi(argv[2]);
    if(down_row < up_row) down_row = up_row;
  }
  run_kana_trainer(kana_type, up_row, down_row, dakuen_handakuen);
}

static void hiragana_training(int argc, char *argv[argc]) {
  kana_training(argc, argv, HIRAGANA);
}
static void katakana_training(int argc, char *argv[argc]) {
  kana_training(argc, argv, KATAKANA);
}

static void add_card(int argc, char *argv[argc]) {
  if(argc < 2) {
    printf("ERROR! too few arguments. Needed at least to: 1) back of the card (Japanese) 2) Front of the card (English)");
    return;
  }
  if(argc == 2) {
    CardDeckManipulation_add_card(strlen(argv[0]), argv[0], strlen(argv[1]), argv[1], 0, NULL);
  }
  else if(argc >= 3) {
    CardDeckManipulation_add_card(strlen(argv[0]), argv[0], strlen(argv[1]), argv[1], strlen(argv[2]), argv[2]);
  }
}

static void cards_training(int argc, char *argv[argc]) {
  (void) argc; (void) argv; 
  run_card_trainer();
}

static void show_cards(int argc, char *argv[argc]) {
  (void) argc; (void) argv; 
  CardDeckManipulation_show_cards();
}

static void find_card(int argc, char *argv[argc]) {
  (void) argc; (void) argv; 
  if(argc < 1) {
    printf("ERROR! too few arguments. Needed at least 1: front or back of the card you are searching for\n");
    return;
  }
  CardDeckManipulation_find_card(strlen(argv[0]), argv[0]);
}

static void edit_card_back(int argc, char *argv[argc]) {
  (void) argc; (void) argv; 
  if(argc != 2) {
    printf("ERROR! there should be exactly 2 arguments: id of the card (int) AND back of the card\n");
    return;
  }
  int id = atoi(argv[0]);
  CardDeckManipulation_edit_card_back(id, strlen(argv[1]), argv[1]);
}

static void edit_card_front(int argc, char *argv[argc]) {
  (void) argc; (void) argv; 
  if(argc != 2) {
    printf("ERROR! there should be exactly 2 arguments: id of the card (int) AND front of the card\n");
    return;
  }
  int id = atoi(argv[0]);
  CardDeckManipulation_edit_card_front(id, strlen(argv[1]), argv[1]);
}



struct Argument listOfArguments[] = {
  { "hiragana",       "start hiragana training",                          hiragana_training }, 
  { "katakana",       "start hiragana training",                          katakana_training }, 
  { "cards",          "start cards training",                             cards_training }, 
  { "add_card",       "add a new card",                                   add_card }, 
  { "show_cards",     "show cards",                                       show_cards }, 
  { "find_card",      "find card",                                        find_card }, 
  { "edit_card_back", "edit back of the card with this id",               edit_card_back }, 
  { "edit_card_front","edit front of the card with this id",              edit_card_front }, 
  //{ "banish_card",    "temporary disable a card",                         cards_training }, 
  //{ "return_card",    "reenable card from banish",                        cards_training }, 
};

int main(int argc, char *argv[argc]) {
  setlocale(LC_ALL, ""); // UTF-8
  initialize_database();
  initialize_random_generator();
  initialize_logger(); 

  if (argc <= 1) {
    //hiragana_train(0, NULL);
    cards_training(0, NULL);
    return 0;
  }
  //first parameter - type of action. Remaning - arguments to that action
  exectute_action_in_Argument_array(ARRAY_SIZE(listOfArguments), listOfArguments, argv[1], argc-2, argv+2);
  return 0;
}
