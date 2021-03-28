// code.c: bytecode tree implementation as defined by code.h
// license information in LICENSE
#include "code.h"
#include "utils.h"
#include <stdlib.h>

Code* code_new (Function** functions, uint32_t functions_len, uint8_t* header,
                uint32_t source_name_len)
{
    Code* code          = malloc (sizeof (Code));
    code->functions     = functions;
    code->functions_len = functions_len;
    code->major         = header[0];
    code->minor         = header[1];
    code->patch         = header[2];
    code->source_name   = ubyte_array_to_string (header, source_name_len);
    return code;
}

void code_free (Code* code)
{
    free (code->source_name);
    for (uint32_t i = 0; i < code->functions_len; i++)
    {
        function_free (code->functions[i]);
    }

    free (code->functions);
    free (code);
}