// main.c: entry point to program
// license information in LICENSE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code.h"
#include "function.h"
#include "opcode.h"
#include "instruction.h"

int main (int argc, char** argv)
{
    uint8_t header[]     = {0xAC, 0x34, 0x34, 0x6d, 0x61, 0x69,
                        0x6e, 0x2e, 0x70, 0x63, 0x62};
    Function** funcs     = malloc (sizeof (Function*));
    Instruction** instrs = malloc (sizeof (Instruction*));
    int8_t args[]        = {};
    instrs[0]            = instruction_new (OP_NOOP, args, 0);
    funcs[0] = function_new (instrs, 1, 0, 0, 0, source_loc_table_new ());
    Code* c  = code_new (funcs, 1, header, 8);
    function_disassemble (funcs[0], stdout);
    code_free (c);
    return 0;
}