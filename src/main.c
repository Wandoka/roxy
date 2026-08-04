#include "hiragana_trainer.h"
#include "args_handling.h"
#include <stdio.h>

void train(int argc, char *argv[]) {
  (void) argc; (void) argv;
  run_trainer();
}

struct Argument listOfArguments[] = {
  { "train",    "start hiragana training",              train }, 
  { 0, 0, 0}
};

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    train(0, NULL);
    return 0;
  }
  //first parameter - type of action. Remaning - arguments to that action
  exectute_action_in_Argument_array(listOfArguments, argv[1], argc-2, argv+2);
  return 0;
}
