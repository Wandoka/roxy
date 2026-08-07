#include "random_generator_interface.h"
#include <stdlib.h>
#include <time.h>


int random_int(int from_inclucive, int till_inclucive) {
  int total = till_inclucive - from_inclucive + 1;
  return from_inclucive + rand()%total;
}

void initialize_random_generator() {
  srand(time(NULL));
}


void random_japan_string(int n, JapanChar all_symbols[n], int m, JapanChar random_string[m], int am) {
  for(int i = 0; i < am; ++i) {
    int j = random_int(0, n);
    random_string[i] = all_symbols[j];
    if(random_int(1, 100) <= 5) { //5% chance of adding a sokuon
      if(can_have_sokuon_before(&random_string[i])) {
        random_string[i].sokuon_before=1;     
      }
    }
  }
}
