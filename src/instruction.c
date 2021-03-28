// instruction.c: instruction implementation as defined by instruction.h
// license information in LICENSE
#include "instruction.h"
#include <stdlib.h>
#include "utils.h"
#include "opcode.h"

Instruction* instruction_new (OpCode op, int8_t* args, uint8_t args_len)
{
    Instruction* i = malloc (sizeof (Instruction));
    i->args        = args;
    i->op          = op;
    i->args_len    = args_len;
    return i;
}

const char* instruction_disassemble (Instruction* instruction)
{
    char* result;
    asprintf (&result, "    %s (%d argument bytes)",
              op_code_to_string (instruction->op), instruction->args_len);

    return result;
}

void instruction_free (Instruction* instruction)
{
    free (instruction);
}