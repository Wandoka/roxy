#include "args_handling.h"
#include "data_base_initialization.h"
#include "common.h"
#include <locale.h>
#include "random_generator_interface.h"
#include "hiragana_trainer.h"

void train(int argc, char *argv[argc]) {
  (void) argc; (void) argv; 
  run_trainer(0, 3);
}

struct Argument listOfArguments[] = {
  { "train", "start hiragana training", train }, 
};

int main(int argc, char *argv[argc]) {
  setlocale(LC_ALL, ""); // UTF-8
  initialize_database();
  initialize_random_generator();
  if (argc <= 1) {
    train(0, NULL);
    return 0;
  }
  //first parameter - type of action. Remaning - arguments to that action
  exectute_action_in_Argument_array(ARRAY_SIZE(listOfArguments), listOfArguments, argv[1], argc-2, argv+2);
  return 0;
}
