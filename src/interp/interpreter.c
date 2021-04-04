// interpreter.c: opcode execution
// license information in LICENSE
#include "interpreter.h"
#include "callstack.h"
#include "stackframe.h"
#include "stack.h"
#include "utils/utils.h"
#include "ir/function.h"
#include "ir/opcode.h"
#include "ir/code.h"
#include "ir/pool.h"
#include <string.h>
#include <stdlib.h>

static void interpreter_execute_instruction (Interpreter* interp,
                                             Instruction* instr, Stack* stack);
static void print_stack (Stack* stack);
static char* primitive_value_to_string (PrimitiveValue* val);

struct _Interpreter
{
    StackFrame* current_frame;
    CallStack* cs;
};

Interpreter* interp_new ()
{
    Interpreter* i   = checked_malloc (sizeof (Interpreter));
    i->cs            = call_stack_new ();
    i->current_frame = NULL;
    return i;
}

void interp_run_function (Interpreter* interp, Function* func)
{
    Stack* locals_stack = stack_new ();
    for (uint32_t i = 0; i < func->instructions_len; i++)
    {
        interpreter_execute_instruction (interp, func->instructions[i],
                                         locals_stack);
    }

    stack_free (locals_stack);
}

static void interpreter_execute_instruction (Interpreter* interp,
                                             Instruction* instr, Stack* stack)
{
    if (instr->op == OP_NOOP)
    {
        return;
    }

    switch (instr->op)
    {
        case OP_IPUSH:
        {
            stack_push (stack, primitive_value_new_i32 (instr->args[0]));
            break;
        }

        case OP_ICONST_0:
        {
            stack_push (stack, primitive_value_new_i32 (0));
            break;
        }

        case OP_ICONST_1:
        {
            stack_push (stack, primitive_value_new_i32 (1));
            break;
        }

        case OP_IADD:
        {
            int32_t val1 = stack_pop (stack)->value.i32;
            int32_t val2 = stack_pop (stack)->value.i32;
            stack_push (stack, primitive_value_new_i32 (val1 + val2));
            break;
        }
    }

    print_stack (stack);
}

static char* primitive_value_to_string (PrimitiveValue* val)
{
    char* value = NULL;
    switch (val->_kind)
    {
        case PVM_PRIMTIVE_TYPE_INT16:
        {
            asprintf (&value, "%d", val->value.i16);
            break;
        }

        case PVM_PRIMTIVE_TYPE_INT32:
        {
            asprintf (&value, "%d", val->value.i32);
            break;
        }

        case PVM_PRIMTIVE_TYPE_UINT8:
        {
            asprintf (&value, "%d", val->value.u8);
            break;
        }

        case PVM_PRIMTIVE_TYPE_UINT16:
        {
            asprintf (&value, "%d", val->value.u16);
            break;
        }

        case PVM_PRIMTIVE_TYPE_UINT32:
        {
            asprintf (&value, "%d", val->value.u32);
            break;
        }

        case PVM_PRIMTIVE_TYPE_INT8:
        {
            asprintf (&value, "%d", val->value.i8);
            break;
        }

        case PVM_PRIMTIVE_TYPE_INT64:
        {
            asprintf (&value, "%li", val->value.i64);
            break;
        }

        case PVM_PRIMTIVE_TYPE_UINT64:
        {
            asprintf (&value, "%lu", val->value.u64);
            break;
        }

        case PVM_PRIMTIVE_TYPE_FLOAT32:
        {
            asprintf (&value, "%f", val->value.f32);
            break;
        }

        case PVM_PRIMTIVE_TYPE_FLOAT64:
        {
            asprintf (&value, "%lf", val->value.f64);
            break;
        }

        case PVM_PRIMTIVE_TYPE_BOOL:
        {
            asprintf (&value, "%s", val->value.b ? "yes" : "no");
            break;
        }

        case PVM_PRIMTIVE_TYPE_CHAR:
        {
            asprintf (&value, "%s", encode_utf8char (val->value.c, NULL));
            break;
        }
    }

    return value;
}

static void print_stack (Stack* stack)
{
    printf ("[");
    for (uint32_t i = 0; i < stack->stack_size - 1; i++)
    {
        printf ("%s ", primitive_value_to_string (stack->values[i]));
    }

    printf ("%s", primitive_value_to_string (stack->top));
    printf ("]\n");
}

void interp_free (Interpreter* interp)
{
    return_if_null (interp);
    call_stack_free (interp->cs);
    stack_frame_free (interp->current_frame);
    free (interp);
}