// instruction.h: instruction definition
// license information in LICENSE
#ifndef __PVM_INSTRUCTION_H__
#define __PVM_INSTRUCTION_H__
#include "opcode.h"
#include <inttypes.h>

typedef struct _Instruction Instruction;

struct _Instruction
{
    OpCode op;
    int32_t* args;
    uint8_t args_len;
};

Instruction* instruction_new (OpCode op, int32_t* args, uint8_t args_len);
const char* instruction_disassemble (Instruction* instruction);
void instruction_free (Instruction* instruction);

#endif /* __PVM_INSTRUCTION_H__ */