// stackemulator.h: stack emulation for bytecode verification
// license information in LICENSE
#ifndef __PVM_STACKEMULATOR_H__
#define __PVM_STACKEMULATOR_H__
#include <inttypes.h>
#include "ir/instruction.h"

typedef struct _StackEmulator StackEmulator;
typedef enum
{
    STACK_EMULATOR_ITEM_TYPE_NONE = 0,
    STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_INT8,
    STACK_EMULATOR_ITEM_TYPE_PRIMTIIVE_INT16,
    STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_INT32,
    STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_INT64,
    STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_UINT8,
    STACK_EMULATOR_ITEM_TYPE_PRIMTIIVE_UINT16,
    STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_UINT32,
    STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_UINT64,
    STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_FLOAT32,
    STACK_EMULATOR_ITEM_TYPE_PRIMTIIVE_FLOAT64,
    STACK_EMULATOR_ITEM_TYPE_PRIMTIIVE_BOOL,
    STACK_EMULATOR_ITEM_TYPE_PRIMTIIVE_CHAR,
    STACK_EMULATOR_ITEM_TYPE_NULLREF,
    STACK_EMULATOR_ITEM_TYPE_OBJECTREF,
} StackEmulatorItemType;

struct _StackEmulator
{
    Instruction** input;
    uint32_t input_length;
    StackEmulatorItemType top_type;
    StackEmulatorItemType* stack;

    // NULL if type != STACK_EMULATOR_ITEM_TYPE_OBJECTREF
    char* classname;
    uint32_t stack_size;
    uint32_t stack_capacity;
};

StackEmulator* stack_emulator_new (Instruction** input, uint32_t input_length);
void stack_emulator_emulate (StackEmulator* e);
void stack_emulator_free (StackEmulator* e);

#endif /* __PVM_STACKEMULATOR_H__ */