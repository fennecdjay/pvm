// opcode.c: opcode helper methods as defined by opcode.h
// license information in LICENSE
#include "opcode.h"
#include "utils.h"

const char* op_code_to_string (OpCode code)
{
    char* result;
    asprintf (&result, "%02X: %s", code, get_op_name (code));
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
    }

    return 0;
}