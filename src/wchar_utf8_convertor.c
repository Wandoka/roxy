#include <assert.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

// wchar_t → UTF-8
void wchar_to_utf8(int n, const wchar_t wstr[n], int m, char utf8[m]) {
    size_t len = wcstombs(utf8, wstr, m);
    assert((int)len != -1);
    assert((int)len<m);
    utf8[len] = '\0';
}

// UTF-8 → wchar_t
void utf8_to_wchar(int n, const char utf8[n], int m, wchar_t wstr[m]) {
    size_t len = mbstowcs(wstr, utf8, m);
    assert((int)len != -1);
    assert((int)len<m);
    wstr[len] = L'\0';
}
