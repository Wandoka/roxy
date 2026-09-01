#include "random_generator_interface.h"
#include "full_hiragana_table.h"
#include "full_katakana_table.h"
#include "japan_char.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>


int random_int(int from_inclucive, int till_inclucive) {
  int total = till_inclucive - from_inclucive + 1;
  return from_inclucive + rand()%total;
}

void initialize_random_generator() {
  srand(time(NULL));
}


void pseudorandom_japan_string(int n, JapanChar all_symbols[n], int m, JapanChar random_jstring[m], int am) {
  /*
   назвать её псевдорандомной нужно я думаю.
   я хочу шанс для секуона, чтобы он попадался где-то раз в 5. Это получается 4% вероятность

   Насчёт kya kyo kyu - я хочу чтобы когда попадалось ki, чтобы оно имело равномерный шанс стать одним из них.

   Также я не хочу чтобы у меня появлялись подряд одинаковые буквы

   Также я хочу чтобы размер всегда был строго 5. Чтобы если были какие-то секуоны или маленькие символы - всё равно в итоге 5 получалось.

   Мб сделать так, чтобы нужно было в all_symbols передать значение
  */
  for(int i = 0; i < am; ++i) {
    int j = random_int(0, n-1);
    random_jstring[i] = all_symbols[j];
    if(i != 0 && wcscmp(random_jstring[i].symbol, random_jstring[i-1].symbol) == 0) {
      //skipping elements that are repeating the previous one
      --i;
      continue;
    }

    //тут идёт проверка на добавление сокуона и йууна
    if(i+1 < am && random_jstring[i].can_sokuon && random_int(1, 100) <= 3) { //3% chance of adding a sokuon
      random_jstring[i+1] = random_jstring[i];
      if(!strcmp("hiragana", random_jstring[i].type)) {
        random_jstring[i] = hiragana_sokuon_jchar;
      }
      else {
        random_jstring[i] = katakana_sokuon_jchar;
      }
      ++i;
    }
    if(i+1 < am && random_jstring[i].can_yoon && random_int(1, 100) <= 20) { //20% chance of adding a yoon
      if(!strcmp("hiragana", random_jstring[i].type)) {
        random_jstring[i+1] = hiragana_yoon_jchars[random_int(0, 2)];
      }
      else {
        random_jstring[i+1] = katakana_yoon_jchars[random_int(0, 2)];
      }
      ++i;
    }
  }
}
