// code.h: bytecode tree definition
// license information in LICENSE
#ifndef __PVM_CODE_H__
#define __PVM_CODE_H__
#include "function.h"
#include "pool.h"
#include <stdio.h>

typedef struct _Code Code;
typedef struct _Header Header;

struct _Code
{
    Function** functions;
    uint32_t functions_len;
    Header* header;
    Pool* pool;
};

Code* code_new (Function** functions, uint32_t functions_len, Header* header,
                Pool* pool);
void code_free (Code* code);

struct _Header
{
    int major;
    int minor;
    int patch;
    char* sourcename;
    char* vendor;
};

Header* header_new (int major, int minor, int patch, char* sourcename,
                    char* vendor);
const char* header_to_string (Header* header);
void header_free (Header* header);

#endif /* __PVM_CODE_H */