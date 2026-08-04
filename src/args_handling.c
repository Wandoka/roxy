#include "args_handling.h"
#include <assert.h>
#include <string.h>

int exectute_action_in_Argument_array(struct Argument listOfArguments[], char* name, int argc, char *argv[]) {
  //listOfArguments should gurantee that it ends with {0, 0, 0}
  int i = 0;
  while(listOfArguments[i].name != 0) {
    if(strcmp(listOfArguments[i].name, name)==0) {
      listOfArguments[i].action(argc, argv); 
      return 0;
    }
    ++i;
  }
  return -1;
}
