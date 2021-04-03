// parser.h: parser definition
// license information in LICENSE
#ifndef __PVM_PARSER_H__
#define __PVM_PARSER_H__

#define PVM_PARSER_POOL_START            0xCB
#define PVM_PARSER_POOL_ENTRY_TYPE_UTF32 0x21
#define PVM_PARSER_POOL_ENTRY_TYPE_LONG  0x22

#include "ir/code.h"
#include "scanner.h"
#include <inttypes.h>

typedef struct _Parser Parser;

struct _Parser
{
    const char* input_filename;
    int8_t* input;
    uint32_t input_len;
    Scanner* scanner;
};

Parser* parser_new (const char* input_filename);
Code* parser_parse (Parser* parser);
void parser_free (Parser* parser);

#endif /* __PVM_PARSER_H__ */