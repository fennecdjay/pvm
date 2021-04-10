// callstack.h: interpreter call stack
// license information in LICENSE
#include "callstack.h"
#include "utils/utils.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

struct _CallStack
{
    StackFrame** frames;
    StackFrame* top;
    uint32_t capacity;
    uint32_t size;
};

CallStack* call_stack_new ()
{
    CallStack* stack = checked_malloc (sizeof (CallStack));
    stack->capacity  = 10;
    stack->frames    = checked_calloc (10, sizeof (StackFrame));
    stack->size      = 0;
    stack->top       = NULL;
    return stack;
}

StackFrame* call_stack_pop (CallStack* stack)
{
    pvm_assert (stack->size != 0, "Cannot pop from empty stack!");
    StackFrame* item = stack->top;
    stack->size--;
    if (stack->size > 1)
    {
        stack->top = stack->frames[stack->size - 1];
    }

    return item;
}

void call_stack_push (CallStack* stack, StackFrame* frame)
{
    if (stack->size == stack->capacity)
    {
        stack->frames = realloc (stack->frames,
                                 sizeof (StackFrame) * (stack->capacity *= 2));
    }

    stack->size++;
    stack->frames[stack->size - 1] = frame;
}

uint32_t call_stack_get_size (CallStack* stack)
{
    return stack->size;
}

void call_stack_free (CallStack* stack)
{
    return_if_null (stack);
    for (uint32_t i = 0; i < ((stack->size < 10) ? 10 : stack->size); i++)
    {
        stack_frame_free (stack->frames[i]);
    }

    free (stack->frames);
    free (stack);
}