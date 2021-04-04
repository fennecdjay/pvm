// stackframe.c: stack frames for function calls
// license information in LICENSE
#include "stackframe.h"
#include "ir/function.h"
#include "utils/utils.h"
#include "interpreter.h"
#include <stdlib.h>

StackFrame* stack_frame_new (Function* func, uint32_t depth)
{
    StackFrame* sf = checked_malloc (sizeof (StackFrame));
    sf->depth      = depth;
    sf->func       = func;
    return sf;
}

uint32_t stack_frame_get_depth (StackFrame* frame)
{
    return frame->depth;
}

void stack_frame_execute (StackFrame* f, Interpreter* interp)
{
    interp_run_function (interp, f->func);
}

void stack_frame_free (StackFrame* frame)
{
    return_if_null (frame);
    function_free (frame->func);
    free (frame);
}