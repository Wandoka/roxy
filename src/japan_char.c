#include "japan_char.h"
#include <assert.h>
#include <string.h>

void wstring_from_japan_chars(int n, JapanChar japanString[n], int m, wchar_t wstring[m], int *wlen) {
  *wlen = 0;
  for(int i = 0; i < n; ++i) {
    JapanChar *jchar = &japanString[i];
    wchar_t *symbol;
    int length;
    symbol = jchar->symbol;
    length = wcslen(symbol);
    assert(*wlen + length < m);
    wcscpy(&wstring[*wlen], symbol);
    *wlen += length; //#чтобы в след раз писал в место, где стоит сейчас "\0" 
  }
  assert(*wlen < m);
  wstring[*wlen] = '\0';
}


void copy_JapanChar(JapanChar* to, JapanChar* from) {
  to->id = from->id; 
  wcscpy(to->symbol, from->symbol);
  strcpy(to->romaji, from->romaji);
  to->row = from->row;
  to->column = from->column;
  strcpy(to->type, from->type);
  strcpy(to->subtype, from->subtype);
  to->can_sokuon = from->can_sokuon;
  to->can_yoon = from->can_yoon;
}

