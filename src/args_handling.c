#include "args_handling.h"
#include <assert.h>
#include <string.h>

int exectute_action_in_Argument_array(int n, Argument listOfArguments[n], char* name, int argc, char *argv[argc]) {
  for(int i = 0; i < n; ++i) {
    if(strcmp(listOfArguments[i].name, name)==0) {
      listOfArguments[i].action(argc, argv); 
      return 0;
    }
  }
  return -1;
}
