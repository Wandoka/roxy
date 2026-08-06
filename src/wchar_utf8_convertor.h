#include <wchar.h>

void wchar_to_utf8(int n, const wchar_t wstr[n], int m, char utf8[m]);
void utf8_to_wchar(int n, const char utf8[n], int m, wchar_t wstr[m]);
