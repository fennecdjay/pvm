// function.h: bytecode function definition
// license information in LICENSE
#ifndef __PVM_FUNCTION_H__
#define __PVM_FUNCTION_H__
#include <stdio.h>
#include "instruction.h"
#include "sourceloctable.h"

typedef struct _Function Function;

struct _Function
{
    Instruction** instructions;
    SourceLocTable* sltable;
    uint32_t instructions_len;
    uint32_t name_ref;
    uint32_t sig_ref;
    uint8_t arity;
};

Function* function_new (Instruction** instructions, uint32_t instructions_len,
                        uint32_t name_ref, uint32_t sig_ref, uint32_t arity,
                        SourceLocTable* sltable);
void function_disassemble (Function* func, FILE* out);
void function_free (Function* function);

#endif /* __PVM_FUNCTION_H */