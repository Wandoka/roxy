#pragma once
#include "japan_char.h"

void set_new_string_to_progress(int n, JapanChar jstring[n]);
void progress_string_by_char(wchar_t c, int *good, int *finished);
void set_new_hidden_string_to_progress(int n, wchar_t hidden[n], int m, wchar_t shown[m]);
