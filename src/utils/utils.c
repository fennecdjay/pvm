// utils.c: general utilities
// written by apachejuice
// license information in LICENSE
#include "utils/utils.h"
#include "utils/asprintf.h"
#include <malloc.h>
#include <stdlib.h>

void* checked_malloc (size_t bytes)
{
    void* ptr = malloc (bytes);
    if (ptr == NULL)
    {
        pvm_panicf ("checked_malloc: Out of memory!");
    }

    return ptr;
}

void* checked_calloc (size_t n, size_t size)
{
    void* ptr = calloc (n, size);
    if (ptr == NULL)
    {
        pvm_panicf ("checked_calloc: Out of memory!");
    }

    return ptr;
}

void* checked_realloc (void* oldptr, size_t newsize)
{
    void* ptr = realloc (oldptr, newsize);
    if (ptr == NULL)
    {
        pvm_panicf ("checked_realloc: Out of memory!");
    }

    return ptr;
}

void pvm_panicf (char* restrict fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
    char* out;
    vasprintf (&out, fmt, ap);
    fprintf (stderr, "%s\nAborting.\n", out);
    free (out);
    va_end (ap);
    exit (1);
}
