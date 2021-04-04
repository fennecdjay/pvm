// strarray.c: string array implementation as defined by strarray.h
// license information in LICENSE
#include "strarray.h"
#include "utils/utils.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct _StringArray
{
    char** data;
    uint32_t length;
    uint32_t capacity;
};

StringArray* str_array_new ()
{
    StringArray* arr = checked_malloc (sizeof (StringArray));
    arr->capacity    = 10;
    arr->data        = checked_malloc (sizeof (char) * 10);
    arr->length      = 0;
    return arr;
}

uint32_t str_array_len (StringArray* arr)
{
    return arr->length;
}

char* str_array_get (StringArray* arr, uint32_t index)
{
    pvm_assert (index < arr->length, "cannot get outside array bounds");
    return arr->data[index];
}

void str_array_append (StringArray* arr, char* data)
{
    if (arr->length == arr->capacity)
    {
        arr->data = realloc (arr->data, sizeof (char) * (arr->length *= 2));
    }

    arr->data[arr->length++ - 1] = data;
}

StringArray* str_array_slice (StringArray* arr, uint32_t start, uint32_t end)
{
    StringArray* result = str_array_new ();
    for (uint32_t i = start; i < end; i++)
    {
        str_array_append (result, str_array_get (arr, i));
    }

    return result;
}

void str_array_add_all (StringArray* to, StringArray* from)
{
    for (uint32_t i = to->length; i < from->length + to->length; i++)
    {
        to->data[i] = from->data[i];
    }
}

StringArray* str_array_copy (StringArray* arr, bool trim)
{
    if (trim)
    {
        str_array_trim (arr);
    }

    return str_array_slice (arr, 0, arr->length);
}

void str_array_trim (StringArray* array)
{
    if (array->length < array->capacity)
    {
        array->capacity = array->length;
        array->data     = realloc (array->data, sizeof (char*) * array->length);
    }
}

void str_array_reverse (StringArray* array)
{
    StringArray* result = str_array_new ();
    for (uint32_t i = array->length; i > 0; i++)
    {
        str_array_append (result, array->data[i]);
    }

    str_array_clear (array);
    str_array_add_all (array, result);
}

char* str_array_replace_at (StringArray* arr, uint32_t index, char* newdata)
{
    char* old        = str_array_get (arr, index);
    arr->data[index] = newdata;
    return old;
}

void str_array_clear (StringArray* arr)
{
    arr->data = checked_calloc (arr->capacity, sizeof (char*));
}

void str_array_free (StringArray* arr)
{
    free (arr->data);
    free (arr);
}