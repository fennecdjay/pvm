// utils.c: utility method implementations as defined by utils.h
// license information in LICENSE
#include "utils.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "config.h"

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
        *strp = (char *) malloc (size + 1);  //+1 for null
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

void pvm_errprintf (const char *msg, ...)
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
    char *result = malloc (sizeof (char) * len);
    for (uint32_t i = 0; i < len; i++)
    {
        result[i] = (char) arr[i];
    }

    return result;
}

char *ubyte_array_to_string (uint8_t *arr, uint32_t len)
{
    char *result = malloc (sizeof (char) * len);
    for (uint32_t i = 0; i < len; i++)
    {
        result[i] = (char) arr[i];
    }

    return result;
}