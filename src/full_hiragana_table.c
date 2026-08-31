#include "full_hiragana_table.h"
#include "data_base_query_templates.h"
#include <ncurses.h>
#include <string.h>

static const wchar_t* hiragana_sokuon_symbol = L"っ";
static const wchar_t* hiragana_yoon_symbols[3] = {
    L"ゃ",   // ya
    L"ゅ",   // yu
    L"ょ"    // yo
};


JapanChar hiragana_sokuon_jchar = {0};
JapanChar hiragana_yoon_jchars[3] = {0};

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


static void add_hiragana_symbol(int n, wchar_t symbol[n], int row, int column, int k, char subtype[k], int can_sokuon, int can_yoon) {

  const char *sql =
    "INSERT INTO JapanChars "
    "(symbol, row, column, subtype, can_sokuon, can_yoon, type) "
    "VALUES (?, ?, ?, ?, ?, ?, ?);"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_wtext(stmt, 1, n, symbol);
  sql_bind_int (stmt,  2, row);
  sql_bind_int (stmt,  3, column);
  sql_bind_text(stmt,  4, k, subtype);
  sql_bind_int (stmt,  5, can_sokuon);
  sql_bind_int (stmt,  6, can_yoon);
  char* hiragana = "hiragana";
  sql_bind_text(stmt,  7, strlen(hiragana), hiragana);

  stmt_run_and_finish(stmt);
}

void fill_full_hiragana_table() {
  //hiragana normal
  for(int i = 0; i < 11; ++i) {
    for(int j = 0; j < 5; ++j) {
      if(hiragana[i][j][0] == L'\0') continue;
      int n = wcslen(hiragana[i][j]);
      char* normal = "normal";
      int k = strlen(normal);
      int can_yoon = (i >= 1 && j == 1); //все ряды кроме гласных, столбец с гласной i
                                         
      add_hiragana_symbol(
          n, (wchar_t *)hiragana[i][j],
          i, j,
          k, normal,
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
      char* dakuten = "dakuten";
      int k = strlen(dakuten);
      int can_yoon = (i >= 1 && j == 1); //все ряды кроме гласных, столбец с гласной i

      add_hiragana_symbol(
          n, (wchar_t *)hiragana_dakuten[i][j],
          i, j,
          k, dakuten,
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
      char* handakuten = "handakuten";
      int k = strlen(handakuten);
      int can_yoon = (i >= 1 && j == 1); //все ряды кроме гласных, столбец с гласной i

      add_hiragana_symbol(
          n, (wchar_t *)hiragana_handakuten[i][j],
          i, j,
          k, (char *)handakuten,
          hiragana_handakuten_sokuon[i][j],
          can_yoon
      );  
    }
  }
  {
    //sokuon
    wcscpy(hiragana_sokuon_jchar.symbol, hiragana_sokuon_symbol);
    strcpy(hiragana_sokuon_jchar.type, "hiragana");
    strcpy(hiragana_sokuon_jchar.subtype, "small");

    int n = wcslen(hiragana_sokuon_jchar.symbol);
    int k = strlen(hiragana_sokuon_jchar.subtype);
    add_hiragana_symbol(
        n, (wchar_t *)hiragana_sokuon_jchar.symbol,
        -1, -1,
        k, hiragana_sokuon_jchar.subtype,
        0,
        0
    );  
  }
  
  for(int i = 0; i < 3; ++i) {
    //yoon
    wcscpy(hiragana_yoon_jchars[i].symbol, hiragana_yoon_symbols[i]);
    strcpy(hiragana_yoon_jchars[i].type, "hiragana");
    strcpy(hiragana_yoon_jchars[i].subtype, "small");
    int n = wcslen(hiragana_yoon_jchars[i].symbol);
    int k = strlen(hiragana_yoon_jchars[i].subtype);
    add_hiragana_symbol(
        n, (wchar_t *)hiragana_yoon_jchars[i].symbol,
        -1, -1,
        k, hiragana_yoon_jchars[i].subtype,
        0,
        0
    );
  }

}
