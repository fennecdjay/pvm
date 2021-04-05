// opcode.c: opcode helper methods as defined by opcode.h
// license information in LICENSE
#include "opcode.h"
#include "utils/utils.h"

const char* op_code_to_string (OpCode code)
{
    char* result;
    asprintf (&result, "%s: (0x%02x)", get_op_name (code), code);
    return result;
}

const char* get_op_name (OpCode code)
{
    switch (code)
    {
        case OP_NOOP: return "noop";
        case OP_IPUSH: return "ipush";
        case OP_ICONST_1: return "iconst_1";
        case OP_ICONST_0: return "iconst_0";
        case OP_IADD: return "iadd";
        case OP_DUP: return "dup";
        case OP_SWAP: return "swap";
    }

    return 0;
}

int8_t opcode_pops (OpCode code)
{
    switch (code)
    {
        case OP_IPUSH:
        case OP_ICONST_0:
        case OP_ICONST_1:
        case OP_SWAP:
        case OP_DUP:
        case OP_NOOP: return 0;

        case OP_IADD: return 2;
    }

    return -1;
}

int8_t opcode_pushes (OpCode code)
{
    switch (code)
    {
        case OP_IPUSH:
        case OP_ICONST_0:
        case OP_IADD:
        case OP_DUP:
        case OP_ICONST_1: return 1;

        case OP_SWAP:
        case OP_NOOP: return 0;
    }

    return -1;
}