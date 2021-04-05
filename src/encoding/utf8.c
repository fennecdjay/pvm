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
#include "utils/utils.h"

char* encode_utf32char (pchar c, uint32_t* len)
{
    // First, convert to UTF-16
    UChar i[8]   = {0};
    UErrorCode e = U_ZERO_ERROR;
    u_strFromUTF32 (i, 4, NULL, &c, 1, &e);
    if (e == U_INVALID_CHAR_FOUND)
    {
        pvm_panicf ("Invalid character in unicode data");
    }
    if (len != NULL)
    {
        *len = u_countChar32 (i, -1);
    }

    // Then convert to UTF-8
    char* dest = checked_malloc (sizeof (char) * 4);
    u_strToUTF8 (dest, 4, NULL, i, sizeof (UChar) * 4, &e);

    return dest;
}

char* encode_utf32str (pstring str, uint32_t* len)
{
    int32_t utf16len;
    UErrorCode e = U_ZERO_ERROR;
    u_strFromUTF32 (NULL, 0, &utf16len, str, -1, &e);
    if (e == U_INVALID_CHAR_FOUND)
    {
        pvm_panicf ("Invalid character in unicode data");
    }

    char* result  = checked_calloc (utf16len * 4, sizeof (char));
    int32_t count = 0;

    for (uint32_t i = 0; i < utf16len; i++)
    {
        char* ch;
        uint32_t len;
        ch = encode_utf32char (str[i], &len);
        strcat (result, ch);
        count += len;
    }

    if (len != NULL)
    {
        *len = utf16len;
    }
    return result;
}

pchar decode_utf8char (char c)
{
    UErrorCode e = U_ZERO_ERROR;
    UChar utf16[1];
    u_strFromUTF8 (utf16, 1, NULL, char_to_string (c), 1, &e);
    if (e == U_INVALID_CHAR_FOUND)
    {
        pvm_panicf ("Invalid character in unicode data");
    }

    pchar result[1];
    u_strToUTF32 (result, 1, NULL, utf16, 1, &e);
    if (e == U_INVALID_CHAR_FOUND)
    {
        pvm_panicf ("Invalid character in unicode data");
    }

    return result[0];
}

pstring decode_utf8str (char* str, uint32_t* len)
{
    size_t str_len = strlen (str);
    if (str_len == 0)
    {
        return L"";
    }

    pstring result = checked_calloc (str_len + 1, sizeof (pchar));
    uint32_t i;
    for (i = 0; i < str_len; i++)
    {
        pchar ch = decode_utf8char (str[i]);
        ;
        result[i] = ch;
    }

    if (len != NULL)
    {
        *len = str_len;
    }

    return result;
}