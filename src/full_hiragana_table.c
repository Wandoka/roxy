#include "full_hiragana_table.h"
#include "data_base_query_templates.h"
#include <ncurses.h>
#include <string.h>

static const wchar_t* sokuon_symbol = L"っ";
static const wchar_t* yoon_symbols[3] = {
    L"ゃ",   // ya
    L"ゅ",   // yu
    L"ょ"    // yo
};

JapanChar sokuon_jchar = {0};
JapanChar yoon_jchars[3] = {0};

// ============================================================
// 1. Hiragana (base + yoon)
// Columns: 0=a 1=i 2=u 3=e 4=o  5=ya 6=yu 7=yo
// ============================================================
static const wchar_t *hiragana[11][5] = {
    { L"あ", L"い", L"う", L"え", L"お"}, // 0  a
    { L"か", L"き", L"く", L"け", L"こ"}, // 1  ka
    { L"さ", L"し", L"す", L"せ", L"そ"}, // 2  sa
    { L"た", L"ち", L"つ", L"て", L"と"}, // 3  ta
    { L"な", L"に", L"ぬ", L"ね", L"の"}, // 4  na
    { L"は", L"ひ", L"ふ", L"へ", L"ほ"}, // 5  ha
    { L"ま", L"み", L"む", L"め", L"も"}, // 6  ma
    { L"や", L"",   L"ゆ", L"",   L"よ"}, // 7  ya
    { L"ら", L"り", L"る", L"れ", L"ろ"}, // 8  ra
    { L"わ", L"",   L"",   L"",   L"を"}, // 9  wa
    { L"ん", L"",   L"",   L"",   L"", }  // 10 n
};

// ============================================================
// 2. Dakuten (゛) + yoon
// ============================================================
static const wchar_t *hiragana_dakuten[11][5] = {
    { L"",   L"",   L"",   L"",   L""   }, // 0  a
    { L"が", L"ぎ", L"ぐ", L"げ", L"ご" }, // 1  ga
    { L"ざ", L"じ", L"ず", L"ぜ", L"ぞ" }, // 2  za
    { L"だ", L"ぢ", L"づ", L"で", L"ど" }, // 3  da
    { L"",   L"",   L"",   L"",   L""   }, // 4  na
    { L"ば", L"び", L"ぶ", L"べ", L"ぼ" }, // 5  ba
    { L"",   L"",   L"",   L"",   L""   }, // 6  ma
    { L"",   L"",   L"",   L"",   L""   }, // 7  ya
    { L"",   L"",   L"",   L"",   L""   }, // 8  ra
    { L"",   L"",   L"",   L"",   L""   }, // 9  wa
    { L"",   L"",   L"",   L"",   L""   }  // 10 n
};

// ============================================================
// 3. Handakuten (゜) + yoon  (only ha-row)
// ============================================================
static const wchar_t *hiragana_handakuten[11][5] = {
    { L"",   L"",   L"",   L"",   L""  }, // 0
    { L"",   L"",   L"",   L"",   L""  }, // 1
    { L"",   L"",   L"",   L"",   L""  }, // 2
    { L"",   L"",   L"",   L"",   L""  }, // 3
    { L"",   L"",   L"",   L"",   L""  }, // 4
    { L"ぱ", L"ぴ", L"ぷ", L"ぺ", L"ぽ"}, // 5  pa
    { L"",   L"",   L"",   L"",   L""  }, // 6
    { L"",   L"",   L"",   L"",   L""  }, // 7
    { L"",   L"",   L"",   L"",   L""  }, // 8
    { L"",   L"",   L"",   L"",   L""  }, // 9
    { L"",   L"",   L"",   L"",   L""  }  // 10
};

// ============================================================
// 4. Sokuon possibility matrices (1 = っ may precede this mora)
// ============================================================
static int hiragana_sokuon[11][5] = {
    { 0,0,0,0,0}, // a
    { 1,1,1,1,1}, // ka / kya…
    { 1,1,1,1,1}, // sa / sha…
    { 1,1,1,1,1}, // ta / cha…
    { 0,0,0,0,0}, // na
    { 0,0,0,0,0}, // ha (plain h- never takes っ)
    { 0,0,0,0,0}, // ma
    { 0,0,0,0,0}, // ya
    { 0,0,0,0,0}, // ra
    { 0,0,0,0,0}, // wa
    { 0,0,0,0,0}  // n
};

static int hiragana_dakuten_sokuon[11][5] = {
    { 0,0,0,0,0}, // a
    { 1,1,1,1,1}, // ga
    { 1,1,1,1,1}, // za
    { 1,1,1,1,1}, // da
    { 0,0,0,0,0}, // na
    { 0,0,0,0,0}, // ba  (っ never before b-)
    { 0,0,0,0,0},
    { 0,0,0,0,0},
    { 0,0,0,0,0},
    { 0,0,0,0,0},
    { 0,0,0,0,0}
};

static int hiragana_handakuten_sokuon[11][5] = {
    { 0,0,0,0,0},
    { 0,0,0,0,0},
    { 0,0,0,0,0},
    { 0,0,0,0,0},
    { 0,0,0,0,0},
    { 1,1,1,1,1}, // pa / pya…  (only place っ is legal for the h-series)
    { 0,0,0,0,0},
    { 0,0,0,0,0},
    { 0,0,0,0,0},
    { 0,0,0,0,0},
    { 0,0,0,0,0}
};

// ============================================================
// 5. English / Hepburn romanization tables
// ============================================================
static const char *hiragana_romaji[11][5] = {
    { "a",  "i",  "u",  "e",  "o" }, // a
    { "ka", "ki", "ku", "ke", "ko"}, // ka
    { "sa", "shi","su", "se", "so"}, // sa
    { "ta", "chi","tsu","te", "to"}, // ta
    { "na", "ni", "nu", "ne", "no"}, // na
    { "ha", "hi", "fu", "he", "ho"}, // ha
    { "ma", "mi", "mu", "me", "mo"}, // ma
    { "ya", "",   "yu", "",   "yo"}, // ya
    { "ra", "ri", "ru", "re", "ro"}, // ra
    { "wa", "",   "",   "",   "wo"}, // wa
    { "n",  "",   "",   "",   "", }  // n
};

static const char *hiragana_dakuten_romaji[11][5] = {
    { "",   "",   "",   "",   ""  },
    { "ga", "gi", "gu", "ge", "go"},
    { "za", "ji", "zu", "ze", "zo"},
    { "da", "ji", "zu", "de", "do"}, // ぢ/づ → ji/zu (Hepburn)
    { "",   "",   "",   "",   ""  },
    { "ba", "bi", "bu", "be", "bo"},
    { "",   "",   "",   "",   ""  },
    { "",   "",   "",   "",   ""  },
    { "",   "",   "",   "",   ""  },
    { "",   "",   "",   "",   ""  },
    { "",   "",   "",   "",   ""  }
};

static const char *hiragana_handakuten_romaji[11][5] = {
    { "",   "",   "",   "",   ""  },
    { "",   "",   "",   "",   ""  },
    { "",   "",   "",   "",   ""  },
    { "",   "",   "",   "",   ""  },
    { "",   "",   "",   "",   ""  },
    { "pa", "pi", "pu", "pe", "po"},
    { "",   "",   "",   "",   ""  },
    { "",   "",   "",   "",   ""  },
    { "",   "",   "",   "",   ""  },
    { "",   "",   "",   "",   ""  },
    { "",   "",   "",   "",   ""  }
};


static void add_hiragana_symbol(int n, wchar_t symbol[n], int m, char romaji[m], int row, int column, int k, char type[k], int can_sokuon, int can_yoon) {

  const char *sql =
    "INSERT INTO hiragana "
    "(symbol, romaji, row, column, type, can_sokuon, can_yoon) "
    "VALUES (?, ?, ?, ?, ?, ?, ?);"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_wtext(stmt, 1, n, symbol);
  sql_bind_text(stmt,  2, m, romaji);
  sql_bind_int (stmt,  3, row);
  sql_bind_int (stmt,  4, column);
  sql_bind_text(stmt,  5, k, type);
  sql_bind_int (stmt,  6, can_sokuon);
  sql_bind_int (stmt,  7, can_yoon);

  stmt_run_and_finish(stmt);
}

void fill_full_hiragana_table() {
  //hiragana normal
  for(int i = 0; i < 11; ++i) {
    for(int j = 0; j < 5; ++j) {
      if(hiragana[i][j][0] == L'\0') continue;
      int n = wcslen(hiragana[i][j]);
      int m = strlen(hiragana_romaji[i][j]);
      int k = strlen("normal");
      int can_yoon = (i >= 1 && j == 2); //все ряды кроме гласных, столбец с гласной i
                                         
      add_hiragana_symbol(
          n, (wchar_t *)hiragana[i][j],
          m, (char *)hiragana_romaji[i][j],
          i, j,
          k, (char *)"normal",
          hiragana_sokuon[i][j],
          can_yoon
      );
    }
  }
  //hiragana_dakuten
  for(int i = 0; i < 11; ++i) {
    for(int j = 0; j < 5; ++j) {
      if (hiragana_dakuten[i][j][0] == L'\0') continue;
      int n = wcslen(hiragana_dakuten[i][j]);
      int m = strlen(hiragana_dakuten_romaji[i][j]);
      int k = strlen("dakuten");
      int can_yoon = (i >= 1 && j == 2); //все ряды кроме гласных, столбец с гласной i

      add_hiragana_symbol(
          n, (wchar_t *)hiragana_dakuten[i][j],
          m, (char *)hiragana_dakuten_romaji[i][j],
          i, j,
          k, (char *)"dakuten",
          hiragana_dakuten_sokuon[i][j],
          can_yoon
      );    
    }
  }
  //hiragana_handakuten
  for(int i = 0; i < 11; ++i) {
    for(int j = 0; j < 5; ++j) {
      if (hiragana_handakuten[i][j][0] == L'\0') continue;
      int n = wcslen(hiragana_handakuten[i][j]);
      int m = strlen(hiragana_handakuten_romaji[i][j]);
      int k = strlen("handakuten");
      int can_yoon = (i >= 1 && j == 2); //все ряды кроме гласных, столбец с гласной i

      add_hiragana_symbol(
          n, (wchar_t *)hiragana_handakuten[i][j],
          m, (char *)hiragana_handakuten_romaji[i][j],
          i, j,
          k, (char *)"handakuten",
          hiragana_handakuten_sokuon[i][j],
          can_yoon
      );  
    }
  }
  {
    //sokuon
    wcscpy(sokuon_jchar.hiragana.symbol, sokuon_symbol);
    strcpy(sokuon_jchar.hiragana.romaji, "sokuon");
    sokuon_jchar.type = SOKUON;

    int n = wcslen(sokuon_jchar.hiragana.symbol);
    int m = strlen(sokuon_jchar.hiragana.romaji);
    int k = strlen("sokuon");
    add_hiragana_symbol(
        n, (wchar_t *)sokuon_jchar.hiragana.symbol,
        m, (char *)sokuon_jchar.hiragana.romaji,
        -1, -1,
        k, (char *)"sokuon",
        0,
        0
    );  
  }
  
  for(int i = 0; i < 3; ++i) {
    //yoon
    wcscpy(yoon_jchars[i].hiragana.symbol, yoon_symbols[i]);
    strcpy(yoon_jchars[i].hiragana.romaji, "yoon");
    yoon_jchars[i].type = YOON;

    int n = wcslen(yoon_jchars[i].hiragana.symbol);
    int m = strlen(yoon_jchars[i].hiragana.romaji);
    int k = strlen("yoon");
    add_hiragana_symbol(
        n, (wchar_t *)yoon_jchars[i].hiragana.symbol,
        m, (char *)yoon_jchars[i].hiragana.romaji,
        -1, -1,
        k, (char *)"yoon",
        0,
        0
    );
  }

}
