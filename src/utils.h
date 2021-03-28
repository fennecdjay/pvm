// utils.h: utility method definitions
// license information in LICENSE
#ifndef __PVM_UTIL_H__
#define __PVM_UTIL_H__
#include <stdarg.h>
#include <inttypes.h>

int vasprintf (char **strp, const char *fmt, va_list ap);
int asprintf (char **strp, const char *fmt, ...);
void pvm_errprintf (const char *msg, ...);
char* byte_array_to_string (int8_t* arr, uint32_t len);
char* ubyte_array_to_string (uint8_t* arr, uint32_t len);

#endif /* __PVM_UTIL_H__ */