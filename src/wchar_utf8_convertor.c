#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>

// wchar_t → UTF-8
char* wchar_to_utf8(const wchar_t* wstr) {
    //плохо, мне не нравится malloc внутри функции
    if (!wstr) return NULL;

    size_t len = wcslen(wstr);
    size_t max_bytes = (len + 1) * 4;   // с запасом

    char* utf8 = malloc(max_bytes);
    if (!utf8) return NULL;

    size_t converted = wcstombs(utf8, wstr, max_bytes);
    if (converted == (size_t)-1) {
        free(utf8);
        return NULL;
    }

    char* result = realloc(utf8, converted + 1);
    return result ? result : utf8;
}

// UTF-8 → wchar_t
wchar_t* utf8_to_wchar(const char* utf8) {
    //плохо, мне не нравится malloc внутри функции
    if (!utf8) return NULL;

    size_t len = strlen(utf8);
    size_t max_wchars = len + 1;

    wchar_t* wstr = malloc(max_wchars * sizeof(wchar_t));
    if (!wstr) return NULL;

    size_t converted = mbstowcs(wstr, utf8, max_wchars);
    if (converted == (size_t)-1) {
        free(wstr);
        return NULL;
    }

    wchar_t* result = realloc(wstr, (converted + 1) * sizeof(wchar_t));
    return result ? result : wstr;
}
