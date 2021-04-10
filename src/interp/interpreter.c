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
static void i32_arithmetic (Stack* stack, OpCode op);

struct _Interpreter
{
    StackFrame* current_frame;
    CallStack* cs;
    Pool* datapool;
    Code* code;
};

Interpreter* interp_new (Code* code)
{
    Interpreter* i   = checked_malloc (sizeof (Interpreter));
    i->cs            = call_stack_new ();
    i->current_frame = NULL;
    i->code          = code;
    i->datapool      = code->pool;
    return i;
}

void interp_run_function (Interpreter* interp, Function* func)
{
    char* name          = interp->datapool->entries[func->name_ref]->value.str;
    Stack* locals_stack = stack_new ();
    interp->current_frame = stack_frame_new (func);
    call_stack_push (interp->cs, interp->current_frame);
    uint32_t depth = call_stack_get_size (interp->cs);
    printf (
        "Interpreter: Entering stack frame at %p for function %s, depth %d\n",
        interp->current_frame, name, depth);

    for (uint32_t i = 0; i < func->instructions_len; i++)
    {
        interpreter_execute_instruction (interp, func->instructions[i],
                                         locals_stack);
    }

    printf (
        "Interpreter: Exited stack frame at %p for function %s, depth: %d\n",
        interp->current_frame, name, depth - 1);
    stack_frame_free (call_stack_pop (interp->cs));
    stack_free (locals_stack);
}

static void interpreter_execute_instruction (Interpreter* interp,
                                             Instruction* instr, Stack* stack)
{
    if (instr == NULL)
    {
        return;
    }

    char* line;
    if (instr->loc == NULL)
    {
        line = "<unknown line>";
    }
    else
    {
        asprintf (&line, "%d", instr->loc->line);
    }

    char* instr_str = instruction_disassemble (instr);
    printf ("Interpreter: running instruction %s (%s:%s)\n", instr_str,
            interp->code->header->sourcename, line);
    free (instr_str);

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

        case OP_DUP:
        {
            PrimitiveValue* value = stack_peek (stack);
            stack_push (stack, value);
            break;
        }

        case OP_SWAP:
        {
            PrimitiveValue* v1 = stack_pop (stack);
            PrimitiveValue* v2 = stack_pop (stack);
            stack_push (stack, v1);
            stack_push (stack, v2);
            break;
        }

        case OP_CALLSIMPLE:
        {
            Function* func =
                interp->code->functions
                    [interp->datapool->entries[instr->args[0]]->value.fref];

            interp_run_function (interp, func);
            break;
        }

        case OP_IADD:
        case OP_IDIV:
        case OP_IMUL:
        case OP_IDREM:
        case OP_ISUB:
        {
            i32_arithmetic (stack, instr->op);
            break;
        }

        case OP_ROT:
        {
            PrimitiveValue* v1 = stack_pop (stack);
            PrimitiveValue* v2 = stack_pop (stack);
            PrimitiveValue* v3 = stack_pop (stack);
            stack_push (stack, v1);
            stack_push (stack, v2);
            stack_push (stack, v3);
            break;
        }

        case OP_ROTN:
        {
            PrimitiveValue* values[instr->args[0]];
            for (uint8_t n = 0; n < instr->args[0]; n++)
            {
                values[n] = stack_pop (stack);
            }

            for (uint8_t n = 0; n < instr->args[0]; n++)
            {
                stack_push (stack, values[n]);
            }

            break;
        }
    }

    print_stack (stack);
}

static void i32_arithmetic (Stack* stack, OpCode op)
{
    int32_t v1 = stack_pop (stack)->value.i32;
    int32_t v2 = stack_pop (stack)->value.i32;
    int32_t result;
    switch (op)
    {
        case OP_IDIV: result = v1 / v2; break;
        case OP_IMUL: result = v1 * v2; break;
        case OP_IDREM: result = v1 % v2; break;
        case OP_IADD: result = v1 + v2; break;
        case OP_ISUB: result = v1 - v2; break;
    }

    stack_push (stack, primitive_value_new_i32 (result));
}

static char* primitive_value_to_string (PrimitiveValue* val)
{
    char* value = NULL;
    switch (val->_kind)
    {
        case PVM_PRIMITIVE_TYPE_INT16:
        {
            asprintf (&value, "%d", val->value.i16);
            break;
        }

        case PVM_PRIMITIVE_TYPE_INT32:
        {
            asprintf (&value, "%d", val->value.i32);
            break;
        }

        case PVM_PRIMITIVE_TYPE_UINT8:
        {
            asprintf (&value, "%d", val->value.u8);
            break;
        }

        case PVM_PRIMITIVE_TYPE_UINT16:
        {
            asprintf (&value, "%d", val->value.u16);
            break;
        }

        case PVM_PRIMITIVE_TYPE_UINT32:
        {
            asprintf (&value, "%d", val->value.u32);
            break;
        }

        case PVM_PRIMITIVE_TYPE_INT8:
        {
            asprintf (&value, "%d", val->value.i8);
            break;
        }

        case PVM_PRIMITIVE_TYPE_INT64:
        {
            asprintf (&value, "%li", val->value.i64);
            break;
        }

        case PVM_PRIMITIVE_TYPE_UINT64:
        {
            asprintf (&value, "%lu", val->value.u64);
            break;
        }

        case PVM_PRIMITIVE_TYPE_FLOAT32:
        {
            asprintf (&value, "%f", val->value.f32);
            break;
        }

        case PVM_PRIMITIVE_TYPE_FLOAT64:
        {
            asprintf (&value, "%lf", val->value.f64);
            break;
        }

        case PVM_PRIMITIVE_TYPE_BOOL:
        {
            asprintf (&value, "%s", val->value.b ? "yes" : "no");
            break;
        }

        case PVM_PRIMITIVE_TYPE_CHAR:
        {
            asprintf (&value, "%s", encode_utf32char (val->value.c, NULL));
            break;
        }
    }

    return value;
}

static void print_stack (Stack* stack)
{
    printf ("Interpreter: stack: [");
    char* str;
    for (uint32_t i = 0; i < stack->stack_size - 1; i++)
    {
        str = primitive_value_to_string (stack->values[i]);
        printf ("%s ", str);
        free (str);
    }

    str = primitive_value_to_string (stack->top);
    printf ("%s", str);
    free (str);
    printf ("]\n");
}

void interp_free (Interpreter* interp)
{
    return_if_null (interp);
    call_stack_free (interp->cs);
    free (interp);
}