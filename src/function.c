// function.h: bytecode function implementation as defined by function.h
// license information in LICENSE
#include "function.h"
#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"
#include "sourceloctable.h"
#include "utils.h"

Function* function_new (Instruction** instructions, uint32_t instructions_len,
                        uint32_t name_ref, uint32_t sig_ref, uint32_t arity,
                        SourceLocTable* sltable)
{
    Function* func         = malloc (sizeof (Function));
    func->arity            = arity;
    func->instructions     = instructions;
    func->instructions_len = instructions_len;
    func->name_ref         = name_ref;
    func->sig_ref          = sig_ref;
    func->sltable          = sltable;
    return func;
}

void function_disassemble (Function* func, FILE* out)
{
    fprintf (out, ".func <#%d> <#%d>\n", func->name_ref, func->sig_ref);
    for (uint32_t i = 0; i < func->instructions_len; i++)
    {
        fprintf (out, "    %s",
                 instruction_disassemble (func->instructions[i]));
    }
    fprintf (out, "\n.end\n");
}

void function_free (Function* function)
{
    return_if_null (function);
    for (uint32_t i = 0; i < function->instructions_len; i++)
    {
        instruction_free (function->instructions[i]);
    }

    source_loc_table_free (function->sltable);
    free (function->instructions);
    free (function);
}