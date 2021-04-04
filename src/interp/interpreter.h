// interpreter.h: opcode execution
// license information in LICENSE
#ifndef __PVM_INTERP_H__
#define __PVM_INTERP_H__
#include "stack.h"
#include "ir/code.h"

typedef struct _Interpreter Interpreter;

Interpreter* interp_new (Code* code);
void interp_run_function (Function* func);
void interp_free (Interpreter* interp);

#endif /* __PVM_INTERP_H__ */