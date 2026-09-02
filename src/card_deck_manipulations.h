#pragma once

typedef enum{
  EDIT_CARD_BACK,
  EDIT_CARD_FRONT,
} CARD_DECK_MANIPULATION_CARD_EDIT;


void CardDeckManipulation_add_card(int n, char back_japan[n], int m, char front_english[m], int k, char type[k]);
void CardDeckManipulation_find_card(int n, char string[n]);
void CardDeckManipulation_show_cards();
void CardDeckManipulation_edit_card_back(int id, int n, char string[n]);
void CardDeckManipulation_edit_card_front(int id, int n, char string[n]);
