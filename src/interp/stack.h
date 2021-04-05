// stack.h: value stack for interpreter
// license information in LICENSE
#ifndef __PVM_STACK_H__
#define __PVM_STACK_H__
#include <inttypes.h>
#include "primvalue.h"

typedef struct _Stack Stack;

struct _Stack
{
    PrimitiveValue** values;
    PrimitiveValue* top;
    uint32_t stack_size;
    uint32_t stack_capacity;
};

Stack* stack_new ();
void stack_push (Stack* stack, PrimitiveValue* value);
PrimitiveValue* stack_pop (Stack* stack);
PrimitiveValue* stack_at (Stack* stack, uint32_t i);
PrimitiveValue* stack_peek (Stack* stack);
PrimitiveValue** stack_pop_n (Stack* stack, uint32_t n);
PrimitiveValue* stack_replace (Stack* stack, uint32_t n, PrimitiveValue* value);
void stack_swap (Stack* stack);
void stack_rotate (Stack* stack);
void stack_rotate_n (Stack* stack, uint32_t n);
void stack_free (Stack* stack);

#endif /* __PVM_STACK_H__ */