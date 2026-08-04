typedef void (*Handler)(int argc, char **argv);

struct Argument {
  char* name;
  char* description;
  Handler action;
};

int exectute_action_in_Argument_array(struct Argument listOfArguments[], char* name, int argc, char *argv[]);  //listOfArguments should gurantee that it ends with {0, 0, 0}

