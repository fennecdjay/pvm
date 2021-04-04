// interpreter.c: opcode execution
// license information in LICENSE
#include "interpreter.h"
#include "stackframe.h"
#include "utils/utils.h"
#include "ir/function.h"
#include "ir/code.h"
#include "ir/pool.h"
#include <string.h>
#include <stdlib.h>

static void interpreter_execute_instruction (Interpreter* interp,
                                             Instruction* instr);

struct _Interpreter
{
    Code* code;
    Function** functions;
    StackFrame* current_frame;
    uint32_t functions_len;
};

Interpreter* interp_new (Code* code)
{
    Interpreter* i   = checked_malloc (sizeof (Interpreter));
    i->functions     = code->functions;
    i->code          = code;
    i->functions_len = code->functions_len;
    return i;
}

void interp_free (Interpreter* interp)
{
    return_if_null (interp);
    for (uint32_t i = 0; i < interp->functions_len; i++)
    {
        function_free (interp->functions[i]);
    }

    code_free (interp->code);
    free (interp->functions);
    free (interp);
}