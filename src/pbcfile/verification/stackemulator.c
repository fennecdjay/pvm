// stackemulator.c: stack emulation for bytecode verification
// license information in LICENSE
#include "stackemulator.h"
#include "utils/utils.h"
#include "ir/opcode.h"
#include "utils/stringbuilder.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static bool instruction_emulate_stack (Instruction* i, StackEmulator* em);
static void stack_push (StackEmulator* stack, StackEmulatorItemType item);
static char* stack_to_string (StackEmulator* se);
static int32_t stack_size_with_no_nones (StackEmulator* se);
static StackEmulatorItemType stack_peek_any (StackEmulator* stack,
                                             Instruction* executor);
static StackEmulatorItemType stack_pop (StackEmulator* stack,
                                        Instruction* executor,
                                        StackEmulatorItemType type);
static StackEmulatorItemType stack_pop_any (StackEmulator* stack,
                                            Instruction* executor);
static void fill_stack_with_none (StackEmulator* s, uint32_t start,
                                  uint32_t end);
static void stack_emulator_error (const char* msg, ...);
static char* instruction_get_bytecode_string (Instruction* instr);

char* stack_emulator_item_type_to_string (StackEmulatorItemType type,
                                          StackEmulator* se)
{
    switch (type)
    {
        case STACK_EMULATOR_ITEM_TYPE_NONE: return "<none>";
        case STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_INT8: return "i8";
        case STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_INT16: return "i16";
        case STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_INT32: return "i32";
        case STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_INT64: return "i64";
        case STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_UINT8: return "u8";
        case STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_UINT16: return "u16";
        case STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_UINT32: return "u32";
        case STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_UINT64: return "u64";
        case STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_FLOAT32: return "f32";
        case STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_FLOAT64: return "f64";
        case STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_BOOL: return "bool";
        case STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_CHAR: return "char";
        case STACK_EMULATOR_ITEM_TYPE_NULLREF: return "nothing";
        case STACK_EMULATOR_ITEM_TYPE_OBJECTREF: return se->classname;
    }

    return NULL;
}

StackEmulator* stack_emulator_new (Instruction** input, uint32_t input_length)
{
    StackEmulator* e  = malloc (sizeof (StackEmulator));
    e->stack_size     = 0;
    e->stack_capacity = 10;
    e->stack          = malloc (sizeof (StackEmulatorItemType) * 10);
    e->classname      = NULL;
    e->input          = input;
    e->input_length   = input_length;

    e->top_type = STACK_EMULATOR_ITEM_TYPE_NONE;
    fill_stack_with_none (e, 0, 10);
    return e;
}

void stack_emulator_emulate (StackEmulator* e)
{
    for (uint32_t i = 0; i < e->input_length; i++)
    {
        instruction_emulate_stack (e->input[i], e);
    }
}

void stack_emulator_free (StackEmulator* e)
{
    return_if_null (e);
    free (e->classname);
    free (e->stack);
    free (e);
}

static void stack_push (StackEmulator* stack, StackEmulatorItemType item)
{
    if (stack->stack_capacity == stack->stack_size)
    {
        stack->stack = realloc (stack->stack, sizeof (StackEmulatorItemType) *
                                                  (stack->stack_capacity *= 2));
    }

    stack->stack[stack->stack_size++] = item;
    stack->top_type                   = item;
}

static StackEmulatorItemType stack_pop (StackEmulator* stack,
                                        Instruction* executor,
                                        StackEmulatorItemType type)
{
    if (stack->stack_size == 0)
    {
        stack_emulator_error (
            "Opcode %s attempting to pop from empty stack!\nBytecode: %s",
            op_code_to_string (executor->op),
            instruction_get_bytecode_string (executor));
    }

    StackEmulatorItemType oldtop = stack->top_type;
    if (oldtop != type)
    {
        printf ("%d\n", type);
        printf ("%d:%s\n", oldtop,
                stack_emulator_item_type_to_string (type, stack));
        stack_emulator_error (
            "Instruction %s cannot be applied to type %s",
            op_code_to_string (executor->op),
            stack_emulator_item_type_to_string (oldtop, stack));
    }

    stack->stack[stack->stack_size - 1] = 0;
    stack->stack_size--;
    if (stack->stack_size > 1)
    {
        stack->top_type = stack->stack[stack->stack_size];
    }

    return oldtop;
}

static bool instruction_emulate_stack (Instruction* i, StackEmulator* se)
{
    if (i == NULL)
    {
        return false;
    }

    switch (i->op)
    {
        case OP_DUP:
        {
            StackEmulatorItemType t = stack_peek_any (se, i);
            stack_push (se, t);
            return true;
        }

        case OP_SWAP:
        {
            StackEmulatorItemType t1 = stack_pop_any (se, i);
            StackEmulatorItemType t2 = stack_pop_any (se, i);
            stack_push (se, t1);
            stack_push (se, t2);
            return true;
        }

        case OP_ICONST_1:
        case OP_IPUSH:
        case OP_ICONST_0:
        {
            stack_push (se, STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_INT32);
            return true;
        }

        case OP_IADD:
        {
            stack_pop (se, i, STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_INT32);
            stack_pop (se, i, STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_INT32);
            stack_push (se, STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_INT32);
            return true;
        }

        case OP_ROTN:
        {
            printf ("args: %d\n", i->args[0]);
            if (i->args[0] < 2)
            {
                pvm_panicf ("Cannot rotn less than 2 items");
            }

            StackEmulatorItemType* types =
                checked_malloc (sizeof (StackEmulatorItemType) * (i)->args[0]);
            for (uint8_t n = 0; n < i->args[0]; n++)
            {
                types[n] = stack_pop_any (se, i);
            }

            for (uint8_t n = 0; n < i->args[0]; n++)
            {
                stack_push (se, types[n]);
            }

            return true;
        }
    }

    return false;
}

static StackEmulatorItemType stack_pop_any (StackEmulator* stack,
                                            Instruction* executor)
{
    if (stack->stack_size == 0)
    {
        stack_emulator_error (
            "Opcode %s attempting to pop from empty stack!\nBytecode: %s",
            op_code_to_string (executor->op),
            instruction_get_bytecode_string (executor));
    }

    return stack->top_type;
}

static StackEmulatorItemType stack_peek_any (StackEmulator* stack,
                                             Instruction* executor)
{
    if (stack->stack_size == 0)
    {
        stack_emulator_error (
            "Opcode %s attempting to peek from empty stack!\nBytecode: %s",
            op_code_to_string (executor->op),
            instruction_get_bytecode_string (executor));
    }

    return stack->top_type;
}

char* instruction_get_bytecode_string (Instruction* instr)
{
    char* output = checked_calloc (5, 3 + sizeof (char) * instr->args_len);
    char* opstr  = malloc (sizeof (char) * 5);
    sprintf (opstr, "%02x ", instr->op);
    strcat (output, opstr);

    for (uint32_t i = 0; i < instr->args_len; i++)
    {
        char* ch = checked_calloc (5, sizeof (char));
        sprintf (ch, "%02x ", instr->args[i]);
        strcat (output, ch);
    }

    return output;
}

static char* stack_to_string (StackEmulator* se)
{
    StringBuilder* sb = str_builder_create ();
    for (uint32_t i = 0; i < se->stack_size; i++)
    {
        char* output;
        size_t len =
            asprintf (&output, se->stack_size - i ? "%s" : "%s ",
                      stack_emulator_item_type_to_string (se->stack[i], se));
        str_builder_add_str (sb, output, len);
    }

    str_builder_add_char (sb, ']');
    return str_builder_dump (sb, NULL);
}

static int32_t stack_size_with_no_nones (StackEmulator* se)
{
    int32_t result = 0;
    for (uint32_t i = 0; i < se->stack_size; i++)
    {
        if (se->stack[i] == STACK_EMULATOR_ITEM_TYPE_NONE)
        {
            break;
        }

        result++;
    }

    return result;
}

static void fill_stack_with_none (StackEmulator* se, uint32_t start,
                                  uint32_t end)
{
    pvm_assert (start < end, "end has to be smaller than start");
    for (uint32_t n = start; n < end; n++)
    {
        se->stack[n] = STACK_EMULATOR_ITEM_TYPE_NONE;
    }
}

static void stack_emulator_error (const char* msg, ...)
{
    va_list ap;
    va_start (ap, msg);
    char* out;
    vasprintf (&out, msg, ap);
    pvm_panicf ("%s\n", out);
}