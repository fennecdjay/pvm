// stackframe.c: stack frames for function calls
// license information in LICENSE
#include "stackframe.h"
#include "ir/function.h"
#include "utils/utils.h"
#include "interpreter.h"
#include <stdlib.h>

StackFrame* stack_frame_new (Function* func)
{
    StackFrame* sf = checked_malloc (sizeof (StackFrame));
    sf->func       = func;
    return sf;
}

void stack_frame_execute (StackFrame* f, Interpreter* interp)
{
    interp_run_function (interp, f->func);
}

void stack_frame_free (StackFrame* frame)
{
    return_if_null (frame);
    free (frame);
}