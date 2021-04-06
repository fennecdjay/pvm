// opcode.h: opcode definitions
// license information in LICENSE
#ifndef __PVM_OPCODE_H__
#define __PVM_OPCODE_H__
#include <inttypes.h>

typedef enum
{
    OP_NOOP = 0x00,
    OP_DUP  = 0x05,
    OP_SWAP = 0x06,
    OP_ROT  = 0x07,
    OP_ROTN = 0x08,

    OP_IPUSH    = 0x10,
    OP_ICONST_1 = 0x11,
    OP_ICONST_0 = 0x12,
    OP_IADD     = 0x13,
    OP_IDIV     = 0x14,
    OP_IMUL     = 0x15,
    OP_IDREM    = 0x16,
    OP_ISUB     = 0x17,
} OpCode;

const char* op_code_to_string (OpCode code);
const char* get_op_name (OpCode code);
int8_t opcode_pops (OpCode code, int32_t* args);
int8_t opcode_pushes (OpCode code, int32_t* args);

#endif /* __PVM_OPCODE_H */