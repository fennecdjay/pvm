// type.h: pvm type representations
// license information in LICENSE
#ifndef __PVM_TYPE_H__
#define __PVM_TYPE_H__
#include "interp/primvalue.h"
#include "encoding/utf8.h"

typedef struct _Type Type;
typedef enum
{
    TYPE_KIND_PRIMITIVE = 0,
    TYPE_KIND_REFERENCE = 1,
    TYPE_KIND_ARRAY     = 2,
} TypeKind;

struct _Type
{
    TypeKind kind;
    union
    {
        pstring refpath;
        PrimitiveValueKind primitive;
        struct
        {
            uint32_t array_depth;
            Type* inner_type;
        } arraydata;
    } label;
};

char get_primitive_type_char (PrimitiveValueKind valuekind);
PrimitiveValueKind get_primitive_type_for_char (char c);

Type* type_new_primitive (char c);
Type* type_new_reference (pstring refpath);
Type* type_new_array (Type* element_type, uint32_t depth);
Type* type_parse (char* str);
char* type_to_string (Type* type);
void type_free (Type* type);

#endif /* __PVM_TYPE_H__ */