// interpreter.h: opcode execution
// license information in LICENSE
#ifndef __PVM_INTERP_H__
#define __PVM_INTERP_H__
#include "ir/function.h"

typedef struct _Interpreter Interpreter;

Interpreter* interp_new ();
void interp_run_function (Interpreter* interp, Function* func);
void interp_free (Interpreter* interp);

#endif /* __PVM_INTERP_H__ */