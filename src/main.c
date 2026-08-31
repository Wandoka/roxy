#include "args_handling.h"
#include "data_base_initialization.h"
#include "common.h"
#include <locale.h>
#include "random_generator_interface.h"
#include "hiragana_trainer.h"
#include "logger.h"
#include "cards_trainer.h"

void hiragana_train(int argc, char *argv[argc]) {
  (void) argc; (void) argv; 
  run_hiragana_trainer(2, 3);
}

void cards_train(int argc, char *argv[argc]) {
  (void) argc; (void) argv; 
  run_card_trainer();
}

struct Argument listOfArguments[] = {
  { "hiragana", "start hiragana training", hiragana_train }, 
  { "cards",    "start cards training"   , cards_train }, 
};

int main(int argc, char *argv[argc]) {
  setlocale(LC_ALL, ""); // UTF-8
  initialize_database();
  initialize_random_generator();
  initialize_logger(); 

  if (argc <= 1) {
    //hiragana_train(0, NULL);
    cards_train(0, NULL);
    return 0;
  }
  //first parameter - type of action. Remaning - arguments to that action
  exectute_action_in_Argument_array(ARRAY_SIZE(listOfArguments), listOfArguments, argv[1], argc-2, argv+2);
  return 0;
}
