// instruction.c: instruction implementation as defined by instruction.h
// license information in LICENSE
#include "instruction.h"
#include <stdlib.h>
#include "utils/utils.h"
#include <stdio.h>
#include "opcode.h"

Instruction* instruction_new (OpCode op, int32_t* args, uint8_t args_len)
{
    Instruction* i = checked_malloc (sizeof (Instruction));
    i->args        = args;
    i->op          = op;
    i->args_len    = args_len;
    return i;
}

const char* instruction_disassemble (Instruction* instruction)
{
    char* result;
    asprintf (&result, "    %s (%d argumens)",
              op_code_to_string (instruction->op), instruction->args_len);

    return result;
}

void instruction_free (Instruction* instruction)
{
    return_if_null (instruction);
    free (instruction->args);
    free (instruction);
}