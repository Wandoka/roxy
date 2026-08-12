#include "adding_cards.h"
#include "data_base_query_templates.h"
#include <string.h>
#include <wchar.h>

static void add_new_card(int n, const wchar_t japanese[n], int m, const wchar_t meaning[m], int k, const char addition_date[k], int q, const char type[k]) {  
  const char *sql =
    "INSERT INTO cards "
    "(japanese, meaning, addition_date, type) "
    "VALUES (?, ?, ?, ?);"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_wtext(stmt, 1, n, japanese);
  sql_bind_wtext(stmt,  2, m, meaning);
  sql_bind_text(stmt,  3, k, addition_date);
  sql_bind_text(stmt,  4, q, type);

  stmt_run_and_finish(stmt);

}

#define LESSON1 "2026-08-03"
#define LESSON2 "2026-08-07"
#define LESSON3 "2026-08-10"
void phrase(const wchar_t japanese[], const wchar_t meaning[], const char addition_date[]) {
  int n = wcslen(japanese);
  int m = wcslen(meaning);
  int k = strlen(addition_date);
  char *type = "phrase";
  int q = strlen(type);
  add_new_card(n, japanese, m, meaning, k, addition_date, q, type);
}
void add_all_cards() {
  //--------------------------
  //--  LESSON 1 
  //--------------------------
  phrase(L"はじめまして", L"Позвольте представиться", LESSON1);
  phrase(L"よろしくおねがいします", L"Надеюсь на ваше хорошее отношение", LESSON1);
  phrase(L"おはよう", L"Доброе утро! (близким)", LESSON1);
  phrase(L"おはようございます", L"Доброе утро! (вежливо)", LESSON1);
  phrase(L"こんにちは", L"Добрый день!", LESSON1);
  phrase(L"こんばんは", L"Добрый вечер!", LESSON1);
  phrase(L"さようなら", L"До свидания! Прощай (если знаем, что долго не увидимся с человеком)", LESSON1);
  phrase(L"それではまた", L"До встречи! (чуть более вежливо)", LESSON1);
  phrase(L"じゃまた", L"До встречи! (менее вежливо)", LESSON1);
  phrase(L"またね", L"До встречи! Увидимся (для неформального общения)", LESSON1);
  phrase(L"またあした", L"До завтра!", LESSON1);
  phrase(L"またらいしゅう", L"До следующей недели!", LESSON1);
  phrase(L"しつれいします", L"Извините, что вас беспокою", LESSON1);
  phrase(L"おさきにしつれいします", L"Извините, что ухожу раньше вас!", LESSON1);
  phrase(L"おつかれさまです", L"Вы сегодня хорошо поработали! (До свидания)", LESSON1);
  phrase(L"おげんきですか", L"Как (ваши) дела?", LESSON1);
  phrase(L"げんきです", L"Хорошо", LESSON1);
  phrase(L"だいじょうぶです", L"Нормально", LESSON1);
  phrase(L"まあまあです", L"Пойдет, 50/50", LESSON1);
  phrase(L"おかげさまでげんきです", L"Вашими молитвами, у меня все хорошо", LESSON1);
  phrase(L"いろいろたいへんです", L"Есть (в жизни) разные сложности", LESSON1);
  phrase(L"いろいろあってね", L"Всякое бывает / это долгая история", LESSON1);
  phrase(L"じゅうにさい", L"12 лет", LESSON1);
  phrase(L"いっさい", L"1 год", LESSON1);
  phrase(L"ななさい", L"7 лет", LESSON1);
  phrase(L"はっさい", L"8 лет", LESSON1);
  phrase(L"じゅっさい", L"10 лет", LESSON1);
  phrase(L"じっさい", L"10 лет", LESSON1);
  phrase(L"はたち", L"20 лет", LESSON1);
  phrase(L"いちねんせい", L"1 класс / курс", LESSON1);
  phrase(L"よねんせい", L"4 класс / курс", LESSON1);
  phrase(L"おなまえはなんですか", L"Как вас зовут?", LESSON1);
  phrase(L"なんさいですか", L"Сколько лет …?", LESSON1);
  phrase(L"おいくつですか", L"Сколько лет …? (вежливо)", LESSON1);
  phrase(L"なんねんせいですか", L"На каком курсе / в каком классе…?", LESSON1);
  phrase(L"せんこうはなんですか", L"Какая у вас специальность?", LESSON1);
  phrase(L"ごしゅっしんはなんですか", L"Откуда вы родом? (вежливо)", LESSON1);
  phrase(L"おしごとはなんですか", L"Какая у вас работа?", LESSON1);

  //--------------------------
  //--  LESSON 2 
  //--------------------------
  phrase(L"にほん", L"Japan", LESSON2);
  phrase(L"あめりか", L"USA", LESSON2);
  phrase(L"いぎりす", L"United Kingdom", LESSON2);
  phrase(L"おーすとらりあ", L"Australia", LESSON2);
  phrase(L"かんこく", L"Korea", LESSON2);
  phrase(L"かなだ", L"Canada", LESSON2);
  phrase(L"ちゅうごく", L"China", LESSON2);
  phrase(L"いんど", L"India", LESSON2);
  phrase(L"えじぷと", L"Egypt", LESSON2);
  phrase(L"ふぃりぴん", L"Philippines", LESSON2);
  phrase(L"じょーじあ", L"Georgia", LESSON2);
  phrase(L"ろしあ", L"Russia", LESSON2);
  phrase(L"うくらいな", L"Ukraine", LESSON2);
  phrase(L"べらるーし", L"Belarus", LESSON2);

  phrase(L"わたし", L"I", LESSON2);
  phrase(L"あなた", L"you", LESSON2);
  phrase(L"ともだち", L"friend", LESSON2);
  phrase(L"～さん", L"polite suffix (Mr./Ms.)", LESSON2);
  phrase(L"なまえ", L"name", LESSON2);

  phrase(L"がくせい", L"student", LESSON2);
  phrase(L"だいがく", L"university", LESSON2);
  phrase(L"だいがくせい", L"university student", LESSON2);
  phrase(L"こうこう", L"high school", LESSON2);
  phrase(L"こうこうせい", L"high school student", LESSON2);
  phrase(L"ちゅうがっこう", L"junior high school", LESSON2);
  phrase(L"ちゅうがくせい", L"junior high student", LESSON2);
  phrase(L"しょうがっこう", L"elementary school", LESSON2);
  phrase(L"しょうがくせい", L"elementary school student", LESSON2);
  phrase(L"りゅうがくせい", L"international / exchange student", LESSON2);
  phrase(L"せんせい", L"teacher", LESSON2);

  phrase(L"いしゃ", L"doctor", LESSON2);
  phrase(L"かいしゃいん", L"office worker / company employee", LESSON2);
  phrase(L"かんごし", L"nurse", LESSON2);
  phrase(L"しゅふ", L"housewife", LESSON2);
  phrase(L"だいがくいんせい", L"graduate student", LESSON2);
  phrase(L"べんごし", L"lawyer", LESSON2);

  phrase(L"おかあさん", L"mother", LESSON2);
  phrase(L"おとうさん", L"father", LESSON2);
  phrase(L"おねえさん", L"older sister", LESSON2);
  phrase(L"おにいさん", L"older brother", LESSON2);
  phrase(L"いもうと", L"younger sister", LESSON2);
  phrase(L"おとうと", L"younger brother", LESSON2);

  phrase(L"あじあけんきゅう", L"Asian studies", LESSON2);
  phrase(L"けいざい", L"economics", LESSON2);
  phrase(L"こうがく", L"engineering", LESSON2);
  phrase(L"こくさいかんけい", L"international relations", LESSON2);
  phrase(L"こんぴゅーたー", L"computer / IT", LESSON2);
  phrase(L"せいじ", L"politics", LESSON2);
  phrase(L"せいぶつがく", L"biology", LESSON2);
  phrase(L"びじねす", L"business", LESSON2);
  phrase(L"ぶんがく", L"literature", LESSON2);
  phrase(L"れきし", L"history", LESSON2);
  phrase(L"にほんご", L"Japanese language", LESSON2);
  phrase(L"えいご", L"English language", LESSON2);

  phrase(L"いま", L"now", LESSON2);
  phrase(L"ごぜん", L"a.m. / morning", LESSON2);
  phrase(L"ごご", L"p.m. / afternoon", LESSON2);
  phrase(L"はん", L"half", LESSON2);

  phrase(L"でんわ", L"telephone", LESSON2);
  phrase(L"ばんごう", L"number", LESSON2);
  phrase(L"なに", L"what", LESSON2);

  phrase(L"あのう", L"um… / excuse me", LESSON2);
  phrase(L"そうです", L"That’s right / Yes, it is", LESSON2);
  phrase(L"そうですか", L"Is that so? / Oh, really?", LESSON2);

  phrase(L"せんこう", L"major", LESSON2);
  phrase(L"にほんじん", L"Japanese people", LESSON2);
  phrase(L"いちじ", L"one o'clock", LESSON2);
  phrase(L"にじはん", L"half past two", LESSON2);
  phrase(L"はい", L"yes", LESSON2);
  phrase(L"～ばん", L"number", LESSON2);
  phrase(L"いちばん", L"первый", LESSON2);
  phrase(L"さいこう", L"самый лучший", LESSON2);
  phrase(L"くに", L"countries", LESSON2);
  phrase(L"しごと", L"occupations", LESSON2);
  phrase(L"かぞく", L"family", LESSON2);


  //--------------------------
  //--  LESSON 3 
  //--------------------------
  phrase(L"ありがとう", L"спасибо", LESSON3);
  phrase(L"ありがとうございます", L"большое спасибо (вежливо)", LESSON3);
  phrase(L"すみません", L"извините", LESSON3);
  phrase(L"いいえ", L"нет", LESSON3);
  phrase(L"ええ", L"да", LESSON3);
  phrase(L"ごめんなさい", L"извините", LESSON3);
  phrase(L"おやすみなさい", L"Спокойной ночи!", LESSON3);
  phrase(L"いってきます", L"Я пошел.", LESSON3);
  phrase(L"いってらっしゃい", L"Возвращайся скорее!", LESSON3);
  phrase(L"ただいま", L"Я пришел.", LESSON3);
  phrase(L"おかえりなさい", L"С возвращением!", LESSON3);
  phrase(L"いただきます", L"Я приступаю (к еде)", LESSON3);
  phrase(L"ごちそうさまでした", L"Спасибо за угощение!", LESSON3);
  phrase(L"がっこう", L"школа / учеба", LESSON3);
  phrase(L"じかん", L"время", LESSON3);
}
