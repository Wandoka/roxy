#include "adding_cards.h"
#include "data_base_query_templates.h"
#include <string.h>
#include <wchar.h>

static void add_new_card(int n, const wchar_t back[n], int m, const wchar_t front[m], int q, const char type[q]) {  
  const char *sql =
    "INSERT INTO cards "
    "(back, front, type) "
    "VALUES (?, ?, ?);"
  ;

  sqlite3_stmt *stmt = sql_prepare(sql);
  sql_bind_wtext(stmt, 1, n, back);
  sql_bind_wtext(stmt,  2, m, front);
  sql_bind_text(stmt,  3, q, type);

  stmt_run_and_finish(stmt);

}

void phrase(const wchar_t back[], const wchar_t front[]) {
  int n = wcslen(back);
  int m = wcslen(front);
  char *type = "phrase";
  int q = strlen(type);
  add_new_card(n, back, m, front, q, type);
}

void add_all_cards() {
  //--------------------------
  //--  LESSON 1 
  //--------------------------
  phrase(L"はじめまして", L"Позвольте представиться");
  phrase(L"よろしくおねがいします", L"Надеюсь на ваше хорошее отношение");
  phrase(L"おはよう", L"Доброе утро! (близким)");
  
  phrase(L"おはようございます", L"Доброе утро! (вежливо)");
  phrase(L"こんにちは", L"Добрый день!");
  phrase(L"こんばんは", L"Добрый вечер!");
  phrase(L"さようなら", L"До свидания! Прощай (если знаем, что долго не увидимся с человеком)");
  phrase(L"それではまた", L"До встречи! (чуть более вежливо)");
  /*
  phrase(L"じゃまた", L"До встречи! (менее вежливо)");
  phrase(L"またね", L"До встречи! Увидимся (для неформального общения)");
  phrase(L"またあした", L"До завтра!");
  phrase(L"またらいしゅう", L"До следующей недели!");
  phrase(L"しつれいします", L"Извините, что вас беспокою");
  phrase(L"おさきにしつれいします", L"Извините, что ухожу раньше вас!");
  phrase(L"おつかれさまです", L"Вы сегодня хорошо поработали! (До свидания)");
  phrase(L"おげんきですか", L"Как (ваши) дела?");
  phrase(L"げんきです", L"Хорошо");
  phrase(L"だいじょうぶです", L"Нормально");
  phrase(L"まあまあです", L"Пойдет, 50/50");
  phrase(L"おかげさまでげんきです", L"Вашими молитвами, у меня все хорошо");
  phrase(L"いろいろたいへんです", L"Есть (в жизни) разные сложности");
  phrase(L"いろいろあってね", L"Всякое бывает / это долгая история");
  phrase(L"じゅうにさい", L"12 лет");
  phrase(L"いっさい", L"1 год");
  phrase(L"ななさい", L"7 лет");
  phrase(L"はっさい", L"8 лет");
  phrase(L"じゅっさい", L"10 лет");
  phrase(L"じっさい", L"10 лет");
  phrase(L"はたち", L"20 лет");
  phrase(L"いちねんせい", L"1 класс / курс");
  phrase(L"よねんせい", L"4 класс / курс");
  phrase(L"おなまえはなんですか", L"Как вас зовут?");
  phrase(L"なんさいですか", L"Сколько лет …?");
  phrase(L"おいくつですか", L"Сколько лет …? (вежливо)");
  phrase(L"なんねんせいですか", L"На каком курсе / в каком классе…?");
  phrase(L"せんこうはなんですか", L"Какая у вас специальность?");
  phrase(L"ごしゅっしんはなんですか", L"Откуда вы родом? (вежливо)");
  phrase(L"おしごとはなんですか", L"Какая у вас работа?");

  //--------------------------
  //--  LESSON 2 
  //--------------------------
  phrase(L"にほん", L"Japan");
  phrase(L"あめりか", L"USA");
  phrase(L"いぎりす", L"United Kingdom");
  phrase(L"おーすとらりあ", L"Australia");
  phrase(L"かんこく", L"Korea");
  phrase(L"かなだ", L"Canada");
  phrase(L"ちゅうごく", L"China");
  phrase(L"いんど", L"India");
  phrase(L"えじぷと", L"Egypt");
  phrase(L"ふぃりぴん", L"Philippines");
  phrase(L"じょーじあ", L"Georgia");
  phrase(L"ろしあ", L"Russia");
  phrase(L"うくらいな", L"Ukraine");
  phrase(L"べらるーし", L"Belarus");

  phrase(L"わたし", L"I");
  phrase(L"あなた", L"you");
  phrase(L"ともだち", L"friend");
  phrase(L"～さん", L"polite suffix (Mr./Ms.)");
  phrase(L"なまえ", L"name");

  phrase(L"がくせい", L"student");
  phrase(L"だいがく", L"university");
  phrase(L"だいがくせい", L"university student");
  phrase(L"こうこう", L"high school");
  phrase(L"こうこうせい", L"high school student");
  phrase(L"ちゅうがっこう", L"junior high school");
  phrase(L"ちゅうがくせい", L"junior high student");
  phrase(L"しょうがっこう", L"elementary school");
  phrase(L"しょうがくせい", L"elementary school student");
  phrase(L"りゅうがくせい", L"international / exchange student");
  phrase(L"せんせい", L"teacher");

  phrase(L"いしゃ", L"doctor");
  phrase(L"かいしゃいん", L"office worker / company employee");
  phrase(L"かんごし", L"nurse");
  phrase(L"しゅふ", L"housewife");
  phrase(L"だいがくいんせい", L"graduate student");
  phrase(L"べんごし", L"lawyer");

  phrase(L"おかあさん", L"mother");
  phrase(L"おとうさん", L"father");
  phrase(L"おねえさん", L"older sister");
  phrase(L"おにいさん", L"older brother");
  phrase(L"いもうと", L"younger sister");
  phrase(L"おとうと", L"younger brother");

  phrase(L"あじあけんきゅう", L"Asian studies");
  phrase(L"けいざい", L"economics");
  phrase(L"こうがく", L"engineering");
  phrase(L"こくさいかんけい", L"international relations");
  phrase(L"こんぴゅーたー", L"computer / IT");
  phrase(L"せいじ", L"politics");
  phrase(L"せいぶつがく", L"biology");
  phrase(L"びじねす", L"business");
  phrase(L"ぶんがく", L"literature");
  phrase(L"れきし", L"history");
  phrase(L"にほんご", L"Japanese language");
  phrase(L"えいご", L"English language");

  phrase(L"いま", L"now");
  phrase(L"ごぜん", L"a.m. / morning");
  phrase(L"ごご", L"p.m. / afternoon");
  phrase(L"はん", L"half");

  phrase(L"でんわ", L"telephone");
  phrase(L"ばんごう", L"number");
  phrase(L"なに", L"what");

  phrase(L"あのう", L"um… / excuse me");
  phrase(L"そうです", L"That’s right / Yes, it is");
  phrase(L"そうですか", L"Is that so? / Oh, really?");

  phrase(L"せんこう", L"major");
  phrase(L"にほんじん", L"Japanese people");
  phrase(L"いちじ", L"one o'clock");
  phrase(L"にじはん", L"half past two");
  phrase(L"はい", L"yes");
  //phrase(L"～ばん", L"number");
  phrase(L"いちばん", L"первый");
  phrase(L"さいこう", L"самый лучший");
  phrase(L"くに", L"countries");
  phrase(L"しごと", L"occupations");
  phrase(L"かぞく", L"family");


  //--------------------------
  //--  LESSON 3 
  //--------------------------
  phrase(L"ありがとう", L"спасибо");
  phrase(L"ありがとうございます", L"большое спасибо (вежливо)");
  phrase(L"すみません", L"извините");
  phrase(L"いいえ", L"нет");
  phrase(L"ええ", L"да");
  phrase(L"ごめんなさい", L"извините");
  phrase(L"おやすみなさい", L"Спокойной ночи!");
  phrase(L"いってきます", L"Я пошел.");
  phrase(L"いってらっしゃい", L"Возвращайся скорее!");
  phrase(L"ただいま", L"Я пришел.");
  phrase(L"おかえりなさい", L"С возвращением!");
  phrase(L"いただきます", L"Я приступаю (к еде)");
  phrase(L"ごちそうさまでした", L"Спасибо за угощение!");
  phrase(L"がっこう", L"школа / учеба");
  phrase(L"じかん", L"время");
  */
}
