// utf8.h: utf8 support
// license information in LICENSE
#ifndef __PVM_UTF8_H__
#define __PVM_UTF8_H__
#include <unicode/umachine.h>

typedef UChar32 pchar;
typedef pchar* pstring;

char* encode_utf32str (pstring str, uint32_t* len);
char* encode_utf32char (pchar c, uint32_t* len);
pchar decode_utf8char (char c);
pstring decode_utf8str (char* str, uint32_t* len);

#endif