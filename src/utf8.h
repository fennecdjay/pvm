// utf8.h: utf8 support
// license information in LICENSE
#ifndef __PVM_UTF8_H__
#define __PVM_UTF8_H__
#include <unicode/umachine.h>

typedef UChar32 pchar;
typedef pchar* pstring;

void encode_utf8str (pstring str, uint64_t str_len, char** outptr);
void encode_utf8char (pchar c, char** outptr, uint32_t* len);

#endif