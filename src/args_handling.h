#pragma once
typedef void (*Handler)(int argc, char **argv);

typedef struct Argument {
  char* name;
  char* description;
  Handler action;
} Argument;

int exectute_action_in_Argument_array(int n, Argument listOfArguments[n], char* name, int argc, char *argv[argc]);

