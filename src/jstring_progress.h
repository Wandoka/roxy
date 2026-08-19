#pragma once
#include "japan_char.h"

void set_new_string_to_Symbol_Progress(int n, JapanChar new_jstring[n]);
void Symbol_Progress_string_by_char(wchar_t c, int *good, int *finished, int *jpos);
void set_new_hidden_string_to_Word_Progress(int n, wchar_t hidden[n], int m, wchar_t shown[m]);
void Word_Progress_string_by_char(wchar_t c, int *good, int *finished);
