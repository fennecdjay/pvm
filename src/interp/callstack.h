// callstack.h: interpreter call stack
// license information in LICENSE
#ifndef __PVM_CALLSTACK_H__
#define __PVM_CALLSTACK_H__
#include "stackframe.h"

typedef struct _CallStack CallStack;

CallStack* call_stack_new ();
void call_stack_push (CallStack* stack, StackFrame* frame);
uint32_t call_stack_get_size (CallStack* stack);
StackFrame* call_stack_pop (CallStack* stack);
void call_stack_free (CallStack* stack);

#endif /* __PVM_CALLSTACK_H__ */