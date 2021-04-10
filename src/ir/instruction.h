// instruction.h: instruction definition
// license information in LICENSE
#ifndef __PVM_INSTRUCTION_H__
#define __PVM_INSTRUCTION_H__
#include "opcode.h"
#include "sourceloctable.h"
#include <inttypes.h>

typedef struct _Instruction Instruction;

struct _Instruction
{
    OpCode op;
    int32_t* args;
    SourceLocTableEntry* loc;
    uint8_t args_len;
};

Instruction* instruction_new (OpCode op, int32_t* args, uint8_t args_len);
char* instruction_disassemble (Instruction* instruction);
int8_t instruction_pops (Instruction* instr);
int8_t instruction_pushes (Instruction* instr);
void instruction_free (Instruction* instruction);

#endif /* __PVM_INSTRUCTION_H__ */