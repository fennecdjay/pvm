// opcode.h: opcode definitions
// license information in LICENSE
#ifndef __PVM_OPCODE_H__
#define __PVM_OPCODE_H__
#include <inttypes.h>

typedef enum
{
    OP_NOOP     = 0x00,
    OP_IPUSH    = 0x01,
    OP_ICONST_1 = 0x02,
    OP_ICONST_0 = 0x03,
    OP_IADD     = 0x04,
    OP_DUP      = 0x05,
    OP_SWAP     = 0x06,
} OpCode;

const char* op_code_to_string (OpCode code);
const char* get_op_name (OpCode code);
int8_t opcode_pops (OpCode code);
int8_t opcode_pushes (OpCode code);

#endif /* __PVM_OPCODE_H */