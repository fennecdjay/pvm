// type.c: type implementation as defined by type.h
// license information in LICENSE
#include "type.h"
#include "utils/utils.h"
#include "utils/stringbuilder.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static Type* new_internal (TypeKind kind);

char get_primitive_type_char (PrimitiveValueKind valuekind)
{
    switch (valuekind)
    {
        case PVM_PRIMITIVE_TYPE_INT16: return 'S';
        case PVM_PRIMITIVE_TYPE_INT32: return 'I';
        case PVM_PRIMITIVE_TYPE_UINT8: return 'X';
        case PVM_PRIMITIVE_TYPE_UINT16: return 'E';
        case PVM_PRIMITIVE_TYPE_UINT32: return 'U';
        case PVM_PRIMITIVE_TYPE_INT8: return 'G';
        case PVM_PRIMITIVE_TYPE_INT64: return 'L';
        case PVM_PRIMITIVE_TYPE_UINT64: return 'K';
        case PVM_PRIMITIVE_TYPE_FLOAT32: return 'F';
        case PVM_PRIMITIVE_TYPE_FLOAT64: return 'D';
        case PVM_PRIMITIVE_TYPE_BOOL: return 'B';
        case PVM_PRIMITIVE_TYPE_CHAR: return 'C';
    }

    pvm_panicf ("Invalid primitive type ID: %d", valuekind);
    return 0;
}

PrimitiveValueKind get_primitive_type_for_char (char c)
{
    switch (c)
    {
        case 'S': return PVM_PRIMITIVE_TYPE_INT16;
        case 'I': return PVM_PRIMITIVE_TYPE_INT32;
        case 'X': return PVM_PRIMITIVE_TYPE_UINT8;
        case 'E': return PVM_PRIMITIVE_TYPE_UINT16;
        case 'U': return PVM_PRIMITIVE_TYPE_UINT32;
        case 'G': return PVM_PRIMITIVE_TYPE_INT8;
        case 'L': return PVM_PRIMITIVE_TYPE_INT64;
        case 'K': return PVM_PRIMITIVE_TYPE_UINT64;
        case 'F': return PVM_PRIMITIVE_TYPE_FLOAT32;
        case 'D': return PVM_PRIMITIVE_TYPE_FLOAT64;
        case 'B': return PVM_PRIMITIVE_TYPE_BOOL;
        case 'C': return PVM_PRIMITIVE_TYPE_CHAR;
    }

    pvm_panicf ("Invalid primitive type character: %c", c);
    return -1;
}

Type* type_new_primitive (char c)
{
    Type* t            = new_internal (TYPE_KIND_PRIMITIVE);
    t->label.primitive = get_primitive_type_for_char (c);
    return t;
}

Type* type_new_reference (pstring refpath)
{
    pvm_assert (
        refpath != NULL && (strcmp (encode_utf32str (refpath, NULL), "") != 0),
        "Invalid reference path");
    Type* t          = new_internal (TYPE_KIND_REFERENCE);
    t->label.refpath = refpath;
    return t;
}

Type* type_new_array (Type* element_type, uint32_t depth)
{
    pvm_assert (depth != 0, "cannot have 0-depth array");
    pvm_assert (depth < 256, "maximum array depth is 255");

    Type* t                        = new_internal (TYPE_KIND_ARRAY);
    t->label.arraydata.array_depth = depth;
    t->label.arraydata.inner_type  = element_type;

    return t;
}

char* type_to_string (Type* type)
{
    char* out;
    switch (type->kind)
    {
        case TYPE_KIND_PRIMITIVE:
        {
            asprintf (&out, "%c",
                      get_primitive_type_char (type->label.primitive));
            break;
        }

        case TYPE_KIND_REFERENCE:
        {
            asprintf (&out, "%s", encode_utf32str (type->label.refpath, NULL));
            break;
        }

        case TYPE_KIND_ARRAY:
        {
            asprintf (&out, "%s%s",
                      type_to_string (type->label.arraydata.inner_type),
                      str_repeat ("]", type->label.arraydata.array_depth));
            break;
        }
    }

    return out;
}

Type* type_parse (char* str)
{
    size_t len = strlen (str);
    pvm_assert (str != NULL && len > 1, "Invalid type string");

    Type* base_type;
    if (str[0] == '#')
    {
        base_type = type_new_primitive (str[1]);
    }
    else if (str[0] == '&')
    {
        uint32_t i;
        for (i = 1; i < len; i++)
        {
            if (str[i] == ']')
            {
                break;
            }
        }

        pvm_assert (str[i - 1] != '/', "Unexpected end of type path");
        base_type = type_new_reference (decode_utf8str (str, NULL));
    }
    else
    {
        pvm_panicf ("Invalid type character %c", str[0]);
    }

    char* ch = strchr (str, ']');
    if (ch == NULL)
    {
        return base_type;
    }

    uint32_t firstbracket = ch - str;
    for (uint32_t i = firstbracket; i < len; i++)
    {
        if (str[i] != ']')
        {
            pvm_panicf ("Expected ] in array type");
        }
    }

    return type_new_array (base_type, len - firstbracket);
}

void type_free (Type* type)
{
    return_if_null (type);
    free (type);
}

static Type* new_internal (TypeKind kind)
{
    Type* t = checked_malloc (sizeof (Type));
    t->kind = kind;
    return t;
}