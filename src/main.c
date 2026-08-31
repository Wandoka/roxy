#include "args_handling.h"
#include "data_base_initialization.h"
#include "common.h"
#include <locale.h>
#include <string.h>
#include "random_generator_interface.h"
#include "kana_trainer.h"
#include "logger.h"
#include "cards_trainer.h"

void kana_training(int argc, char *argv[argc]) {
  (void) argc; (void) argv; 
  KANA_TYPE kana_type = KATAGANA;
  DAKUEN_HANDAKUEN dakuen_handakuen = NO_DAKUEN_HANDAKUEN;
  int up_row   = 0;
  int down_row = 10;
  if(argc >= 1 && !strcmp(argv[0], "hiragana")) {
    kana_type = HIRAGANA;
  }
  if(argc >= 2) {
    up_row = stoi(argv[1]);
  }
  if(argc >= 3) {
    up_row = stoi(argv[2]);
  }
  if(argc >= 4 && strcmp(argv[3], "+")) {
    dakuen_handakuen = YES_DAKUEN_HANDAKUEN;
  }
  run_kana_trainer(kana_type, up_row, down_row, dakuen_handakuen);
}

void cards_training(int argc, char *argv[argc]) {
  (void) argc; (void) argv; 
  run_card_trainer();
}

struct Argument listOfArguments[] = {
  { "kana", "start kana training", kana_training }, 
  { "cards","start cards training"   , cards_training }, 
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
