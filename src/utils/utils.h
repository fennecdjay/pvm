// utils.h: general utilities
// written by apachejuice
// license information in LICENSE
#ifndef __pvm_utils_h__
#define __pvm_utils_h__
#include <stddef.h>

void* checked_malloc (size_t bytes);
void* checked_calloc (size_t n, size_t size);
void* checked_realloc (void* oldptr, size_t newsize);
void pvm_panicf (char* restrict fmt, ...);

#endif
