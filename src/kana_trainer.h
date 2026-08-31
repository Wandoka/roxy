#pragma once

int run_kana_trainer(int up_row, int down_row);

typedef enum {
  HIRAGANA,
  KATAGANA,
} KANA_TYPE;

typedef enum {
  NO_DAKUEN_HANDAKUEN,
  YES_DAKUEN_HANDAKUEN,
} DAKUEN_HANDAKUEN;
