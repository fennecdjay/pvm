// stack.h: value stack for interpreter
// license information in LICENSE
#include "stack.h"
#include "utils/utils.h"
#include <stdlib.h>

Stack* stack_new ()
{
    Stack* s          = checked_malloc (sizeof (Stack));
    s->stack_capacity = 10;
    s->stack_size     = 0;
    s->values         = checked_malloc (sizeof (PrimitiveValue) * 10);
    s->top            = checked_malloc (sizeof (PrimitiveValue));
    return s;
}

void stack_push (Stack* stack, PrimitiveValue* value)
{
    if (stack->stack_size == stack->stack_capacity)
    {
        stack->values =
            realloc (stack->values,
                     sizeof (PrimitiveValue) * (stack->stack_capacity *= 2));
    }

    stack->top = value;
    stack->stack_size++;
    stack->values[stack->stack_size - 1] = value;
}

PrimitiveValue* stack_pop (Stack* stack)
{
    pvm_assert (stack->stack_size != 0, "cannot pop from empty stack");

    PrimitiveValue* result = stack->top;
    stack->stack_size--;
    primitive_value_free (stack->values[stack->stack_size]);
    stack->top = stack->values[stack->stack_size - 1];
    return result;
}

PrimitiveValue** stack_pop_n (Stack* stack, uint32_t n)
{
    pvm_assert (n != 0, "cannot pop 0 items from stack");
    PrimitiveValue** result = checked_malloc (sizeof (PrimitiveValue*) * n);
    for (uint32_t i = 0; i < n; i++)
    {
        result[i] = stack_pop (stack);
    }

    return result;
}

void stack_swap (Stack* stack)
{
    PrimitiveValue* first  = stack_pop (stack);
    PrimitiveValue* second = stack_pop (stack);
    stack_push (stack, first);
    stack_push (stack, second);
}

void stack_rotate (Stack* stack)
{
    PrimitiveValue* first = stack_pop (stack);
    PrimitiveValue* second =
        stack_replace (stack, stack->stack_size - 2, first);
    stack_push (stack, second);
}

PrimitiveValue* stack_at (Stack* stack, uint32_t n)
{
    pvm_assert (stack->stack_size > n, "stack does not have required index");
    return stack->values[n];
}

PrimitiveValue* stack_replace (Stack* stack, uint32_t n, PrimitiveValue* value)
{
    PrimitiveValue* item = stack_at (stack, n);
    stack->values[n]     = value;
    return item;
}

void stack_rotate_n (Stack* stack, uint32_t n)
{
    pvm_assert (n > 1, "cannot rotate less than 2 items");

    if (n == 2)
    {
        stack_swap (stack);
    }
    else if (n == 3)
    {
        stack_rotate (stack);
    }
    else
    {
        PrimitiveValue** cache = checked_malloc (sizeof (PrimitiveValue) * n);
        for (uint32_t i = 0; i < n; i++)
        {
            cache[i] = stack_pop (stack);
        }

        for (uint32_t k = n; k > 0; k--)
        {
            stack_push (stack, cache[k]);
        }
    }
}

void stack_free (Stack* stack)
{
    return_if_null (stack);
    primitive_value_free (stack->top);
    for (uint32_t i = 0; i < stack->stack_size; i++)
    {
        primitive_value_free (stack->values[i]);
    }

    free (stack->values);
    free (stack);
}