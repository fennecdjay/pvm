// code.h: bytecode tree definition
// license information in LICENSE
#ifndef __PVM_CODE_H__
#define __PVM_CODE_H__
#include "function.h"
#include <stdio.h>

typedef struct _Code Code;

struct _Code
{
    Function** functions;
    uint32_t functions_len;

    char* source_name;
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
};

Code* code_new (Function** functions, uint32_t functions_len, uint8_t* header,
                uint32_t source_name_len);
void code_free (Code* code);

#endif /* __PVM_CODE_H */