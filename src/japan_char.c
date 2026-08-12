#include "japan_char.h"
#include "src/logger.h"
#include <assert.h>
#include <stdlib.h>

void unite_hiragana_kanji_katagana(int n, Hiragana hiragana[n], int m, Katagana katagana[m], int k, Kanji kanji[k], int q, JapanChar japan_string[q], int *am) {
  *am = 0;
  for(int i = 0; i < n; ++i) {
    assert(*am < q);
    japan_string[*am].type = HIRAGANA;
    japan_string[*am].hiragana = hiragana[i];
    *am += 1;
  }
  for(int i = 0; i < m; ++i) {
    assert(*am < q);
    japan_string[*am].type = KATAGANA;
    japan_string[*am].katagana = katagana[i];
    *am += 1;
  }
  for(int i = 0; i < k; ++i) {
    assert(*am < q);
    japan_string[*am].type = KANJI;
    japan_string[*am].kanji = kanji[i];
    *am += 1;
  }

}

void wstring_from_japan_chars(int n, JapanChar japan_string[n], int m, wchar_t wstring[m], int *wlen) {
  *wlen = 0;
  for(int i = 0; i < n; ++i) {
    JapanChar *jchar = &japan_string[i];
    wchar_t *symbol;
    int length;
    switch(jchar->type) {
      case HIRAGANA:
      case SOKUON:
      case YOON:
        symbol = japan_string[i].hiragana.symbol;
        break;
      case KATAGANA:
        exit(1);
        break;
      case KANJI:
        exit(1);
        break;
      default:
        LOG("WANDOKA_3");
        exit(1);
    }
    length = wcslen(symbol);
    assert(*wlen + length < m);
    wcscpy(&wstring[*wlen], symbol);
    *wlen += length; //#чтобы в след раз писал в место, где стоит сейчас "\0" 
  }
  assert(*wlen < m);
  wstring[*wlen] = '\0';
}

int can_have_sokuon_before(JapanChar *c) {
  switch(c->type) {
    case HIRAGANA:
    case SOKUON:
    case YOON:
      return c->hiragana.can_sokuon; 
      break;
    case KATAGANA:
      return 0;
      break;
    case KANJI:
      return 0;
    default:
      LOG("WANDOKA_4");
      exit(1);
  }
}
