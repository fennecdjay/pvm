// utf8.h: utf8 support
// license information in LICENSE
#ifndef __PVM_UTF8_H__
#define __PVM_UTF8_H__
#include <unicode/umachine.h>

typedef UChar32 pchar;
typedef pchar* pstring;

char* encode_utf8str (pstring str, uint64_t str_len);
char* encode_utf8char (pchar c, uint32_t* len);

#endif