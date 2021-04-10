// opcode.c: opcode helper methods as defined by opcode.h
// license information in LICENSE
#include "opcode.h"
#include "utils/utils.h"

static char* opcode_names[] = {
    [OP_NOOP] = "noop",         [OP_DUP] = "dup",
    [OP_SWAP] = "swap",         [OP_ROT] = "rot",
    [OP_ROTN] = "rotn",         [OP_IPUSH] = "ipush",
    [OP_ICONST_1] = "iconst_1", [OP_ICONST_0] = "iconst_0",
    [OP_IADD] = "iadd",         [OP_IDIV] = "idiv",
    [OP_IMUL] = "imul",         [OP_IDREM] = "idrem",
    [OP_ISUB] = "isub",         [OP_CALLSIMPLE] = "callsimple",

};

char* op_code_to_string (OpCode code)
{
    char* result;
    asprintf (&result, "%s: (0x%02x)", get_op_name (code), code);
    return result;
}

char* get_op_name (OpCode code)
{
    return opcode_names[code];
}