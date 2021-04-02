// utils.h: utility method definitions
// license information in LICENSE
#ifndef __PVM_UTIL_H__
#define __PVM_UTIL_H__
#define return_if_null(x) \
    if ((!x))             \
    {                     \
        return;           \
    }

#define pvm_assert(x, msg)                                                 \
    if (!(x))                                                              \
    {                                                                      \
        pvm_panicf ("%s: line %d, function %s: Assertion '%s' failed: %s", \
                    __FILE__, __LINE__, __FUNCTION__, #x, msg);            \
    }

#include <stdarg.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdbool.h>

int vasprintf (char **strp, const char *fmt, va_list ap);
int asprintf (char **strp, const char *fmt, ...);
void pvm_panicf (const char *msg, ...);
char *byte_array_to_string (int8_t *arr, uint32_t len);
char *ubyte_array_to_string (uint8_t *arr, uint32_t len);
void* checked_malloc (size_t size);

uint16_t swap_u16 (uint16_t val);
int16_t swap_i16 (int16_t val);
uint32_t swap_u32 (uint32_t val);
int32_t swap_i32 (int32_t val);
int64_t swap_i64 (int64_t val);
uint64_t swap_u64 (uint64_t val);

#endif /* __PVM_UTIL_H__ */