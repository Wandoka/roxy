#pragma once

typedef enum {
  HIRAGANA,
  KATAKANA,
} KANA_TYPE;

typedef enum {
  NO_DAKUEN_HANDAKUEN,
  YES_DAKUEN_HANDAKUEN,
} DAKUEN_HANDAKUEN;

int run_kana_trainer(KANA_TYPE kana_type, int up_row, int down_row, DAKUEN_HANDAKUEN dakuen_handakuen);
