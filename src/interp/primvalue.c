// primvalue.c: interpreter runtime primitive values
// license information in LICENSE
#include "primvalue.h"
#include "utils/utils.h"
#include <stdlib.h>

static PrimitiveValue* new_internal (PrimitiveValueKind kind);

PrimitiveValue* primitive_value_new_i8 (int8_t value)
{
    PrimitiveValue* pv = new_internal (PVM_PRIMITIVE_TYPE_INT8);
    pv->value.i8       = value;
    return pv;
}

PrimitiveValue* primitive_value_new_i16 (int16_t value)
{
    PrimitiveValue* pv = new_internal (PVM_PRIMITIVE_TYPE_INT16);
    pv->value.i16      = value;
    return pv;
}

PrimitiveValue* primitive_value_new_i32 (int32_t value)
{
    PrimitiveValue* pv = new_internal (PVM_PRIMITIVE_TYPE_INT32);
    pv->value.i32      = value;
    return pv;
}

PrimitiveValue* primitive_value_new_i64 (int64_t value)
{
    PrimitiveValue* pv = new_internal (PVM_PRIMITIVE_TYPE_INT64);
    pv->value.i64      = value;
    return pv;
}

PrimitiveValue* primitive_value_new_u8 (uint8_t value)
{
    PrimitiveValue* pv = new_internal (PVM_PRIMITIVE_TYPE_UINT8);
    pv->value.u8       = value;
    return pv;
}

PrimitiveValue* primitive_value_new_u16 (uint16_t value)
{
    PrimitiveValue* pv = new_internal (PVM_PRIMITIVE_TYPE_UINT16);
    pv->value.u16      = value;
    return pv;
}

PrimitiveValue* primitive_value_new_u32 (uint32_t value)
{
    PrimitiveValue* pv = new_internal (PVM_PRIMITIVE_TYPE_UINT32);
    pv->value.u32      = value;
    return pv;
}

PrimitiveValue* primitive_value_new_u64 (uint64_t value)
{
    PrimitiveValue* pv = new_internal (PVM_PRIMITIVE_TYPE_UINT64);
    pv->value.u64      = value;
    return pv;
}

PrimitiveValue* primitive_value_new_f32 (float value)
{
    PrimitiveValue* pv = new_internal (PVM_PRIMITIVE_TYPE_FLOAT32);
    pv->value.f32      = value;
    return pv;
}

PrimitiveValue* primitive_value_new_f64 (double value)
{
    PrimitiveValue* pv = new_internal (PVM_PRIMITIVE_TYPE_FLOAT64);
    pv->value.f64      = value;
    return pv;
}

PrimitiveValue* primitive_value_new_bool (bool value)
{
    PrimitiveValue* pv = new_internal (PVM_PRIMITIVE_TYPE_BOOL);
    pv->value.b        = value;
    return pv;
}

PrimitiveValue* primitive_value_new_char (UChar32 value)
{
    PrimitiveValue* pv = new_internal (PVM_PRIMITIVE_TYPE_CHAR);
    pv->value.c        = value;
    return pv;
}

void primitive_value_free (PrimitiveValue* value)
{
    return_if_null (value);
    free (value);
}

static PrimitiveValue* new_internal (PrimitiveValueKind kind)
{
    PrimitiveValue* value = checked_malloc (sizeof (PrimitiveValue));
    value->_kind          = kind;
    value->copy           = false;
    return value;
}