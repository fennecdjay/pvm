// utils.c: utility method implementations as defined by utils.h
// license information in LICENSE
#include "utils.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define insane_free(ptr) \
    {                    \
        free (ptr);      \
        ptr = 0;         \
    }
// Only linux has vasprintf, so were going to implement it here.
int vasprintf (char **strp, const char *fmt, va_list ap)
{
    int r = -1, size;

    va_list ap2;
    va_copy (ap2, ap);

    size = vsnprintf (0, 0, fmt, ap2);

    if ((size >= 0) && (size < INT_MAX))
    {
        *strp = (char *) checked_malloc (size + 1);  //+1 for null
        if (*strp)
        {
            r = vsnprintf (*strp, size + 1, fmt, ap);  //+1 for null
            if ((r < 0) || (r > size))
            {
                insane_free (*strp);
                r = -1;
            }
        }
    }
    else
    {
        *strp = 0;
    }

    va_end (ap2);

    return (r);
}

void *checked_malloc (size_t size)
{
    void *ptr = malloc (size);
    pvm_assert (ptr != NULL, "Not enough memory");
    return ptr;
}

int asprintf (char **strp, const char *fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
    char *msg;
    int n = vasprintf (&msg, fmt, ap);
    va_end (ap);
    *strp = msg;
    return n;
}

void pvm_panicf (const char *msg, ...)
{
    char *fmt;
    va_list ap;
    va_start (ap, msg);
    vasprintf (&fmt, msg, ap);
    va_end (ap);
    printf ("Error: %s\n", fmt);
    free (fmt);
    exit (1);
}

char *byte_array_to_string (int8_t *arr, uint32_t len)
{
    char *result = checked_malloc (sizeof (arr));
    memcpy (result, arr, sizeof (int8_t) * len);
    return result;
}

char *ubyte_array_to_string (uint8_t *arr, uint32_t len)
{
    char *result = checked_malloc (sizeof (char) * len);
    memcpy (result, arr, sizeof (uint8_t) * len);
    return result;
}

uint16_t swap_u16 (uint16_t val)
{
    return (val << 8) | (val >> 8);
}

int16_t swap_i16 (int16_t val)
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

uint32_t swap_u32 (uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

int32_t swap_i32 (int32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | ((val >> 16) & 0xFFFF);
}

int64_t swap_i64 (int64_t val)
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL) |
          ((val >> 8) & 0x00FF00FF00FF00FFULL);
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL) |
          ((val >> 16) & 0x0000FFFF0000FFFFULL);
    return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
}

uint64_t swap_u64 (uint64_t val)
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL) |
          ((val >> 8) & 0x00FF00FF00FF00FFULL);
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL) |
          ((val >> 16) & 0x0000FFFF0000FFFFULL);
    return (val << 32) | (val >> 32);
}