// stackemulator.c: stack emulation for bytecode verification
// license information in LICENSE
#include "stackemulator.h"
#include "utils/utils.h"
#include "ir/opcode.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static bool instruction_emulate_stack (Instruction* i, StackEmulator* em);
static void stack_push (StackEmulator* stack, StackEmulatorItemType item);
static StackEmulatorItemType stack_pop (StackEmulator* stack,
                                        Instruction* executor);
static void stack_emulator_error (const char* msg, ...);
static char* instruction_get_bytecode_string (Instruction* instr);

StackEmulator* stack_emulator_new (Instruction** input, uint32_t input_length)
{
    StackEmulator* e  = malloc (sizeof (StackEmulator));
    e->stack_size     = 0;
    e->stack_capacity = 10;

    // Using calloc because we want everything initialized to 0
    // which is equal to STACK_EMULATOR_ITEM_TYPE_NONE
    e->stack        = calloc (10, sizeof (StackEmulatorItemType));
    e->classname    = NULL;
    e->input        = input;
    e->input_length = input_length;
    stack_pop (e, input[input_length - 1]);
    return e;
}

void stack_emulator_emulate (StackEmulator* e)
{
    for (uint32_t i = 0; i < e->input_length; i++)
    {
    }
}

void stack_emulator_free (StackEmulator* e)
{
    return_if_null (e);
    free (e->input);
    free (e->classname);
    free (e);
}

static void stack_push (StackEmulator* stack, StackEmulatorItemType item)
{
    if (stack->stack_capacity == stack->stack_size)
    {
        stack->stack = realloc (stack->stack, sizeof (StackEmulatorItemType) *
                                                  (stack->stack_capacity *= 2));
    }

    stack->stack[stack->stack_size++ - 1] = item;
}

static StackEmulatorItemType stack_pop (StackEmulator* stack,
                                        Instruction* executor)
{
    if (stack->stack_size == 0)
    {
        stack_emulator_error (
            "Opcode 0x%02X attempting to pop from empty stack!\nBytecode: %s",
            instruction_get_bytecode_string (executor));
    }

    StackEmulatorItemType oldtop        = stack->top_type;
    stack->stack[stack->stack_size - 1] = 0;
    stack->stack_size--;
    if (stack->stack_size != 1)
    {
        stack->top_type = stack->stack[stack->stack_size - 1];
    }

    return oldtop;
}

static bool instruction_emulate_stack (Instruction* i, StackEmulator* se)
{
    switch (i->op)
    {
        case OP_NOOP:
        {
            return false;
        }

        case OP_ICONST_1:
        case OP_IPUSH:
        case OP_ICONST_0:
        {
            stack_push (se, STACK_EMULATOR_ITEM_TYPE_PRIMITIVE_INT32);
            return true;
        }
    }

    return false;
}

char* instruction_get_bytecode_string (Instruction* instr)
{
    char* result = malloc (3 + sizeof (char) * (instr->args_len * 3));
    sprintf (result, "%02x ", instr->op);

    for (uint32_t i = 0; i < instr->args_len; i++)
    {
        char* oldresult = malloc (sizeof (result));
        strcpy (oldresult, result);
        sprintf (result, "%s%02x ", oldresult, instr->args[i]);
    }

    return result;
}

static void stack_emulator_error (const char* msg, ...)
{
    va_list ap;
    va_start (ap, msg);
    char* out;
    vasprintf (&out, msg, ap);
    pvm_panicf ("%s\n", out);
}