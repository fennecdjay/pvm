// utf8.c: utf8 support
// license information in LICENSE
#include "utf8.h"
#include <unicode/umachine.h>
#include <unicode/urename.h>
#include <unicode/ustring.h>
#include <stdlib.h>
#include <stdio.h>
#include <unicode/utypes.h>
#include <string.h>
#include "utils.h"

char* encode_utf8char (pchar c, uint32_t* len)
{
    // First, convert to UTF-16
    UChar i[8] = {0};
    UErrorCode e = U_ZERO_ERROR;
    u_strFromUTF32 (i, 4, NULL, &c, 1, &e);
    if (e == U_INVALID_CHAR_FOUND)
    {
        pvm_panicf ("Invalid character in unicode data");
    }
    *len = u_countChar32 (i, -1);

    // Then convert to UTF-8
    char* dest = malloc (sizeof (char) * 4);
    u_strToUTF8 (dest, 4, NULL, i, sizeof (UChar) * 4, &e);

    return dest;
}

char* encode_utf8str (pstring str, uint64_t str_len)
{
    char* result = malloc (sizeof (char) * str_len * 4);
    int32_t count = 0;

    for (uint64_t i = 0; i < str_len; i++)
    {
        char* ch;
        uint32_t len;
        ch = encode_utf8char (str[i], &len);
        for (uint32_t n = 0; n < len; n++)
        {
            result[count++] = ch[n];
        }
    }

    return result;
}