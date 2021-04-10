// primvalue.h: interpreter runtime primitive values
// license information in LICENSE
#ifndef __PVM_PRIMVALUE_H__
#define __PVM_PRIMVALUE_H__
#include <inttypes.h>
#include <stdbool.h>
#include "encoding/utf8.h"

typedef struct _PrimitiveValue PrimitiveValue;
typedef enum
{
    PVM_PRIMITIVE_TYPE_INT8 = 0,
    PVM_PRIMITIVE_TYPE_INT16,
    PVM_PRIMITIVE_TYPE_INT32,
    PVM_PRIMITIVE_TYPE_INT64,
    PVM_PRIMITIVE_TYPE_UINT8,
    PVM_PRIMITIVE_TYPE_UINT16,
    PVM_PRIMITIVE_TYPE_UINT32,
    PVM_PRIMITIVE_TYPE_UINT64,
    PVM_PRIMITIVE_TYPE_FLOAT32,
    PVM_PRIMITIVE_TYPE_FLOAT64,
    PVM_PRIMITIVE_TYPE_BOOL,
    PVM_PRIMITIVE_TYPE_CHAR,
} PrimitiveValueKind;

struct _PrimitiveValue
{
    PrimitiveValueKind _kind;
    uint32_t ref_count;
    union
    {
        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;

        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;

        float f32;
        double f64;

        bool b;
        pchar c;
    } value;
};

PrimitiveValue* primitive_value_new_i8 (int8_t value);
PrimitiveValue* primitive_value_new_i16 (int16_t value);
PrimitiveValue* primitive_value_new_i32 (int32_t value);
PrimitiveValue* primitive_value_new_i64 (int64_t value);

PrimitiveValue* primitive_value_new_u8 (uint8_t value);
PrimitiveValue* primitive_value_new_u16 (uint16_t value);
PrimitiveValue* primitive_value_new_u32 (uint32_t value);
PrimitiveValue* primitive_value_new_u64 (uint64_t value);

PrimitiveValue* primitive_value_new_f32 (float value);
PrimitiveValue* primitive_value_new_f64 (double value);

PrimitiveValue* primitive_value_new_bool (bool value);
PrimitiveValue* primitive_value_new_char (UChar32 value);

void primitive_value_free (PrimitiveValue* value);

#endif /* __PVM_PRIMVALUE_H__ */