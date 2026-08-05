#include "args_handling.h"
#include "data_base_initialization.h"
#include"data_base_interface.h"

void train(int argc, char *argv[]) {
  (void) argc; (void) argv;
  initialize_database();
  
  Hiragana listOfHiragana[100];
  int am = 0;
  select_hiragana_rows(listOfHiragana, &am, 100, 0, 0);
  for(int i = 0; i < am; ++i) {
    wprintf(L"found romaji %l\n", listOfHiragana[i].romaji);
    wprintf(L"found symbol %ls %d\n", listOfHiragana[i].symbol, listOfHiragana[i].symbol);
  }

  //run_trainer();
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
