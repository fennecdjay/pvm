// utils.c: utility method implementations as defined by utils.h
// license information in LICENSE
#include "utils.h"

#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "config.h"

#ifndef HAVE_VASPRINTF
#define insane_free(ptr) \
    {                    \
        free (ptr);      \
        ptr = 0;         \
    }
int vasprintf (char **strp, const char *fmt, va_list ap)
{
    int r = -1, size;

    va_list ap2;
    va_copy (ap2, ap);

    size = vsnprintf (0, 0, fmt, ap2);

    if ((size >= 0) && (size < INT_MAX))
    {
        *strp = (char *)malloc (size + 1);  //+1 for null
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
#else
int vasprintf (char **strp, const char *fmt, va_list ap);
#endif

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