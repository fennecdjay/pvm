// opcode.h: opcode definitions
// license information in LICENSE
#ifndef __PVM_OPCODE_H__
#define __PVM_OPCODE_H__

typedef enum
{
    OP_NOOP     = 0x00,
    OP_IPUSH    = 0x01,
    OP_ICONST_1 = 0x02,
    OP_ICONST_0 = 0x03
} OpCode;

const char* op_code_to_string (OpCode code);
const char* get_op_name (OpCode code);

#endif /* __PVM_OPCODE_H */