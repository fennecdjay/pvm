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
    i->loc         = NULL;
    return i;
}

char* instruction_disassemble (Instruction* instruction)
{
    char* result;
    char* opstr = op_code_to_string (instruction->op);
    asprintf (&result, "    %s (%d arguments)", opstr, instruction->args_len);

    free (opstr);
    return result;
}

int8_t instruction_pops (Instruction* instr)
{
    switch (instr->op)
    {
        case OP_IPUSH:
        case OP_ICONST_0:
        case OP_ICONST_1:
        case OP_IDIV:
        case OP_IMUL:
        case OP_IDREM:
        case OP_ISUB:
        case OP_NOOP: return 0;
        case OP_DUP: return 1;
        case OP_SWAP:
        case OP_IADD:
        case OP_ROT: return 2;
        case OP_ROTN: return instr->args[0];
        case OP_CALLSIMPLE: return instr->args_len - 1;
    }

    return -1;
}

int8_t instruction_pushes (Instruction* instr)
{
    switch (instr->op)
    {
        case OP_IPUSH:
        case OP_ICONST_0:
        case OP_ICONST_1:
        case OP_IDIV:
        case OP_IMUL:
        case OP_IDREM:
        case OP_DUP:
        case OP_IADD:
        case OP_CALLSIMPLE:
        case OP_ISUB: return 1;
        case OP_NOOP: return 0;
        case OP_SWAP:
        case OP_ROT: return 2;
        case OP_ROTN: return instr->args[0];
    }

    return -1;
}

void instruction_free (Instruction* instruction)
{
    return_if_null (instruction);
    free (instruction->args);
    free (instruction);
}