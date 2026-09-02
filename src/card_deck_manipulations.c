#include "card_deck_manipulations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "wchar_utf8_convertor.h"
#include "data_base_interface.h"

void CardDeckManipulation_add_card(int n, char back_japan[n], int m, char front_english[m], int k, char type[k]) {
  wchar_t wstring_back_japan[512] = {0};
  utf8_to_wchar(strlen(back_japan), back_japan, ARRAY_SIZE(wstring_back_japan), wstring_back_japan);

  wchar_t wstring_front_english[512] = {0};
  utf8_to_wchar(strlen(front_english), front_english, ARRAY_SIZE(wstring_front_english), wstring_front_english);

  char result_type[128] = "phrase";
  if(k != 0) {
    strcpy(result_type, type);
  }

  add_new_card(ARRAY_SIZE(wstring_front_english), wstring_front_english, 
               ARRAY_SIZE(wstring_back_japan),    wstring_back_japan, 
               ARRAY_SIZE(result_type),           result_type);
}


void CardDeckManipulation_find_card(int n, char string[n]) {
  wchar_t wstring[512] = {0};
  utf8_to_wchar(strlen(string), string, ARRAY_SIZE(wstring), wstring);

  Card card;
  if(0 != find_card_by_wstring(&card, ARRAY_SIZE(wstring), wstring)) {
    printf("was not able to find a card :(\n"); 
    return;
  }
  printf("id = %d\n", card.id);
  printf("back = %ls\n", card.back);
  printf("front = %ls\n", card.front);
}

void CardDeckManipulation_show_cards() {
  system("sqlite3 -box ~/.local/share/roxy/roxy_sqlite3.db \"SELECT * FROM Cards;\"");
}

void CardDeckManipulation_edit_card_back(int id, int n, char string[n]) {
  wchar_t wstring[512] = {0};
  utf8_to_wchar(strlen(string), string, ARRAY_SIZE(wstring), wstring);
  update_card_back(id, n, wstring);
}
void CardDeckManipulation_edit_card_front(int id, int n, char string[n]) {
  wchar_t wstring[512] = {0};
  utf8_to_wchar(strlen(string), string, ARRAY_SIZE(wstring), wstring);
  update_card_front(id, n, wstring);
}
