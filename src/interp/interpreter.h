// interpreter.h: opcode execution
// license information in LICENSE
#ifndef __PVM_INTERP_H__
#define __PVM_INTERP_H__
#include "ir/function.h"
#include "ir/pool.h"

typedef struct _Interpreter Interpreter;

Interpreter* interp_new (Pool* datapool);
void interp_run_function (Interpreter* interp, Function* func);
void interp_free (Interpreter* interp);

#endif /* __PVM_INTERP_H__ */