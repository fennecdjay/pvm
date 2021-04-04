// strarray.h: string array definition
// license information in license
#ifndef __PVM_STRARRAY_H__
#define __PVM_STRARRAY_H__
#define foreach_strarr(arr, m) \
    for (uint32_t m = 0; m < str_array_len (arr); m++)

#include <inttypes.h>
#include <stdbool.h>

typedef struct _StringArray StringArray;

StringArray* str_array_new ();
uint32_t str_array_len (StringArray* arr);
char* str_array_get (StringArray* arr, uint32_t index);
void str_array_add_all (StringArray* to, StringArray* from);
void str_array_append (StringArray* arr, char* data);
StringArray* str_array_slice (StringArray* arr, uint32_t start, uint32_t end);
StringArray* str_array_copy (StringArray* arr, bool trim);
void str_array_trim (StringArray* array);
void str_array_reverse (StringArray* array);
char* str_array_replace_at (StringArray* arr, uint32_t index, char* newdata);
void str_array_clear (StringArray* arr);
void str_array_free (StringArray* arr);

#endif /* __PVM_STRARRAY_H__ */