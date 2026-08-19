#include "full_katakana_table.h"
#include "data_base_query_templates.h"
#include <ncurses.h>
#include <string.h>

static const wchar_t* katakana_sokuon_symbol = L"ッ";

static const wchar_t* katakana_yoon_symbols[3] = {
    L"ャ", // ya
    L"ュ", // yu
    L"ョ"  // yo
};

JapanChar katakana_sokuon_jchar = {0};
JapanChar katakana_yoon_jchars[3] = {0};

// ============================================================
// 1. Katakana (base + yoon)
// Columns: 0=a 1=i 2=u 3=e 4=o
// ============================================================
static const wchar_t *katakana[11][5] = {
    { L"ア", L"イ", L"ウ", L"エ", L"オ" }, // 0 a
    { L"カ", L"キ", L"ク", L"ケ", L"コ" }, // 1 ka
    { L"サ", L"シ", L"ス", L"セ", L"ソ" }, // 2 sa
    { L"タ", L"チ", L"ツ", L"テ", L"ト" }, // 3 ta
    { L"ナ", L"ニ", L"ヌ", L"ネ", L"ノ" }, // 4 na
    { L"ハ", L"ヒ", L"フ", L"ヘ", L"ホ" }, // 5 ha
    { L"マ", L"ミ", L"ム", L"メ", L"モ" }, // 6 ma
    { L"ヤ", L"",  L"ユ", L"",  L"ヨ" }, // 7 ya
    { L"ラ", L"リ", L"ル", L"レ", L"ロ" }, // 8 ra
    { L"ワ", L"",  L"",  L"",  L"ヲ" }, // 9 wa
    { L"ン", L"",  L"",  L"",  L""  }  // 10 n
};

// ============================================================
// 2. Dakuten (゛) + yoon
// ============================================================
static const wchar_t *katakana_dakuten[11][5] = {
    { L"",  L"",  L"",  L"",  L""  }, // 0 a
    { L"ガ", L"ギ", L"グ", L"ゲ", L"ゴ" }, // 1 ga
    { L"ザ", L"ジ", L"ズ", L"ゼ", L"ゾ" }, // 2 za
    { L"ダ", L"ヂ", L"ヅ", L"デ", L"ド" }, // 3 da
    { L"",  L"",  L"",  L"",  L""  }, // 4 na
    { L"バ", L"ビ", L"ブ", L"ベ", L"ボ" }, // 5 ba
    { L"",  L"",  L"",  L"",  L""  }, // 6 ma
    { L"",  L"",  L"",  L"",  L""  }, // 7 ya
    { L"",  L"",  L"",  L"",  L""  }, // 8 ra
    { L"",  L"",  L"",  L"",  L""  }, // 9 wa
    { L"",  L"",  L"",  L"",  L""  }  // 10 n
};

// ============================================================
// 3. Handakuten (゜) + yoon (only ha-row)
// ============================================================
static const wchar_t *katakana_handakuten[11][5] = {
    { L"",  L"",  L"",  L"",  L""  }, // 0
    { L"",  L"",  L"",  L"",  L""  }, // 1
    { L"",  L"",  L"",  L"",  L""  }, // 2
    { L"",  L"",  L"",  L"",  L""  }, // 3
    { L"",  L"",  L"",  L"",  L""  }, // 4
    { L"パ", L"ピ", L"プ", L"ペ", L"ポ" }, // 5 pa
    { L"",  L"",  L"",  L"",  L""  }, // 6
    { L"",  L"",  L"",  L"",  L""  }, // 7
    { L"",  L"",  L"",  L"",  L""  }, // 8
    { L"",  L"",  L"",  L"",  L""  }, // 9
    { L"",  L"",  L"",  L"",  L""  }  // 10
};

// ============================================================
// 4. Sokuon possibility matrices (1 = ッ may precede this mora)
// ============================================================
static int katakana_sokuon[11][5] = {
    { 0,0,0,0,0 }, // a
    { 1,1,1,1,1 }, // ka / kya…
    { 1,1,1,1,1 }, // sa / sha…
    { 1,1,1,1,1 }, // ta / cha…
    { 0,0,0,0,0 }, // na
    { 0,0,0,0,0 }, // ha (plain h- never takes ッ)
    { 0,0,0,0,0 }, // ma
    { 0,0,0,0,0 }, // ya
    { 0,0,0,0,0 }, // ra
    { 0,0,0,0,0 }, // wa
    { 0,0,0,0,0 }  // n
};

static int katakana_dakuten_sokuon[11][5] = {
    { 0,0,0,0,0 }, // a
    { 1,1,1,1,1 }, // ga
    { 1,1,1,1,1 }, // za
    { 1,1,1,1,1 }, // da
    { 0,0,0,0,0 }, // na
    { 0,0,0,0,0 }, // ba (ッ never before b-)
    { 0,0,0,0,0 },
    { 0,0,0,0,0 },
    { 0,0,0,0,0 },
    { 0,0,0,0,0 },
    { 0,0,0,0,0 }
};

static int katakana_handakuten_sokuon[11][5] = {
    { 0,0,0,0,0 },
    { 0,0,0,0,0 },
    { 0,0,0,0,0 },
    { 0,0,0,0,0 },
    { 0,0,0,0,0 },
    { 1,1,1,1,1 }, // pa / pya… (only place ッ is legal for the h-series)
    { 0,0,0,0,0 },
    { 0,0,0,0,0 },
    { 0,0,0,0,0 },
    { 0,0,0,0,0 },
    { 0,0,0,0,0 }
};

// ============================================================
// 5. English / Hepburn romanization tables
// ============================================================
static const char *katakana_romaji[11][5] = {
    { "a",  "i",  "u",  "e",  "o"  }, // a
    { "ka", "ki", "ku", "ke", "ko" }, // ka
    { "sa", "shi","su", "se", "so" }, // sa
    { "ta", "chi","tsu","te", "to" }, // ta
    { "na", "ni", "nu", "ne", "no" }, // na
    { "ha", "hi", "fu", "he", "ho" }, // ha
    { "ma", "mi", "mu", "me", "mo" }, // ma
    { "ya", "",   "yu", "",   "yo" }, // ya
    { "ra", "ri", "ru", "re", "ro" }, // ra
    { "wa", "",   "",   "",   "wo" }, // wa
    { "n",  "",   "",   "",   ""   }  // n
};

static const char *katakana_dakuten_romaji[11][5] = {
    { "",   "",   "",   "",   ""   },
    { "ga", "gi", "gu", "ge", "go" },
    { "za", "ji", "zu", "ze", "zo" },
    { "da", "ji", "zu", "de", "do" }, // ヂ/ヅ → ji/zu (Hepburn)
    { "",   "",   "",   "",   ""   },
    { "ba", "bi", "bu", "be", "bo" },
    { "",   "",   "",   "",   ""   },
    { "",   "",   "",   "",   ""   },
    { "",   "",   "",   "",   ""   },
    { "",   "",   "",   "",   ""   },
    { "",   "",   "",   "",   ""   }
};

static const char *katakana_handakuten_romaji[11][5] = {
    { "",   "",   "",   "",   ""   },
    { "",   "",   "",   "",   ""   },
    { "",   "",   "",   "",   ""   },
    { "",   "",   "",   "",   ""   },
    { "",   "",   "",   "",   ""   },
    { "pa", "pi", "pu", "pe", "po" },
    { "",   "",   "",   "",   ""   },
    { "",   "",   "",   "",   ""   },
    { "",   "",   "",   "",   ""   },
    { "",   "",   "",   "",   ""   },
    { "",   "",   "",   "",   ""   }
};

static void add_katakana_symbol(int n, wchar_t symbol[n], int m, char romaji[m],
                                int row, int column, int k, char subtype[k],
                                int can_sokuon, int can_yoon)
{
    const char *sql =
        "INSERT INTO JapanChars "
        "(symbol, romaji, row, column, subtype, can_sokuon, can_yoon, type) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt = sql_prepare(sql);

    sql_bind_wtext(stmt, 1, n, symbol);
    sql_bind_text (stmt, 2, m, romaji);
    sql_bind_int  (stmt, 3, row);
    sql_bind_int  (stmt, 4, column);
    sql_bind_text (stmt, 5, k, subtype);
    sql_bind_int  (stmt, 6, can_sokuon);
    sql_bind_int  (stmt, 7, can_yoon);

    const char* katakana_type = "katakana";
    sql_bind_text(stmt, 8, strlen(katakana_type), katakana_type);

    stmt_run_and_finish(stmt);
}

void fill_full_katakana_table()
{
    // katakana normal
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (katakana[i][j][0] == L'\0') continue;

            int n = wcslen(katakana[i][j]);
            int m = strlen(katakana_romaji[i][j]);
            char* normal = "normal";
            int k = strlen(normal);
            int can_yoon = (i >= 1 && j == 1); // все ряды кроме гласных, столбец i

            add_katakana_symbol(
                n, (wchar_t *)katakana[i][j],
                m, (char *)katakana_romaji[i][j],
                i, j,
                k, normal,
                katakana_sokuon[i][j],
                can_yoon
            );
        }
    }

    // katakana_dakuten
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (katakana_dakuten[i][j][0] == L'\0') continue;

            int n = wcslen(katakana_dakuten[i][j]);
            int m = strlen(katakana_dakuten_romaji[i][j]);
            char* dakuten = "dakuten";
            int k = strlen(dakuten);
            int can_yoon = (i >= 1 && j == 1);

            add_katakana_symbol(
                n, (wchar_t *)katakana_dakuten[i][j],
                m, (char *)katakana_dakuten_romaji[i][j],
                i, j,
                k, dakuten,
                katakana_dakuten_sokuon[i][j],
                can_yoon
            );
        }
    }

    // katakana_handakuten
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (katakana_handakuten[i][j][0] == L'\0') continue;

            int n = wcslen(katakana_handakuten[i][j]);
            int m = strlen(katakana_handakuten_romaji[i][j]);
            char* handakuten = "handakuten";
            int k = strlen(handakuten);
            int can_yoon = (i >= 1 && j == 1);

            add_katakana_symbol(
                n, (wchar_t *)katakana_handakuten[i][j],
                m, (char *)katakana_handakuten_romaji[i][j],
                i, j,
                k, handakuten,
                katakana_handakuten_sokuon[i][j],
                can_yoon
            );
        }
    }

    // sokuon
    {
        wcscpy(katakana_sokuon_jchar.symbol, katakana_sokuon_symbol);
        strcpy(katakana_sokuon_jchar.romaji, "");
        strcpy(katakana_sokuon_jchar.type, "katakana");
        strcpy(katakana_sokuon_jchar.subtype, "small");

        int n = wcslen(katakana_sokuon_jchar.symbol);
        int m = strlen(katakana_sokuon_jchar.romaji);
        int k = strlen(katakana_sokuon_jchar.subtype);

        add_katakana_symbol(
            n, (wchar_t *)katakana_sokuon_jchar.symbol,
            m, (char *)katakana_sokuon_jchar.romaji,
            -1, -1,
            k, katakana_sokuon_jchar.subtype,
            0,
            0
        );
    }

    // yoon (small ya/yu/yo)
    for (int i = 0; i < 3; ++i) {
        wcscpy(katakana_yoon_jchars[i].symbol, katakana_yoon_symbols[i]);
        strcpy(katakana_yoon_jchars[i].romaji, "");
        strcpy(katakana_yoon_jchars[i].type, "katakana");
        strcpy(katakana_yoon_jchars[i].subtype, "small");

        int n = wcslen(katakana_yoon_jchars[i].symbol);
        int m = strlen(katakana_yoon_jchars[i].romaji);
        int k = strlen(katakana_yoon_jchars[i].subtype);

        add_katakana_symbol(
            n, (wchar_t *)katakana_yoon_jchars[i].symbol,
            m, (char *)katakana_yoon_jchars[i].romaji,
            -1, -1,
            k, katakana_yoon_jchars[i].subtype,
            0,
            0
        );
    }
}
