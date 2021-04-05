// stackframe.h: stack frames for function calls
// license information in LICENSE
#ifndef __PVM_STACKFRAME_H__
#define __PVM_STACKFRAME_H__
#include "ir/function.h"
#include "interpreter.h"

typedef struct _StackFrame StackFrame;

struct _StackFrame
{
    Function* func;
};

StackFrame* stack_frame_new (Function* func);
void stack_frame_execute (StackFrame* f, Interpreter* interp);
void stack_frame_free (StackFrame* frame);

#endif /* __PVM_STACKFRAME_H__ */