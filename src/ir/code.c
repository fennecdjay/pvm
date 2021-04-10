// code.c: bytecode tree implementation as defined by code.h
// license information in LICENSE
#include "code.h"
#include "utils/utils.h"
#include "pool.h"
#include <stdlib.h>

Code* code_new (Function** functions, uint32_t functions_len, Header* header,
                Pool* pool)
{
    Code* code          = checked_malloc (sizeof (Code));
    code->functions     = functions;
    code->functions_len = functions_len;
    code->header        = header;
    code->pool          = pool;
    return code;
}

void code_free (Code* code)
{
    return_if_null (code);
    for (uint32_t i = 0; i < code->functions_len; i++)
    {
        function_free (code->functions[i]);
    }

    free (code->functions);
    pool_free (code->pool);
    header_free (code->header);
    free (code);
}

Header* header_new (int major, int minor, int patch, char* sourcename,
                    char* vendor)
{
    Header* h     = checked_malloc (sizeof (Header));
    h->major      = major;
    h->minor      = minor;
    h->patch      = patch;
    h->sourcename = sourcename;
    h->vendor     = vendor == NULL ? "<none>" : vendor;
    return h;
}

char* header_to_string (Header* header)
{
    char* c;
    asprintf (&c,
              "Header[maj=0x%02X min=0x%02X patch=0x%02X srcname=%s vendor=%s]",
              header->major, header->minor, header->patch, header->sourcename,
              header->vendor);
    return c;
}

void header_free (Header* header)
{
    free (header->sourcename);
    free (header->vendor);
    free (header);
}