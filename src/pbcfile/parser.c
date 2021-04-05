// parser.c: parser implementation as defined by parser.h
// license information in LICENSE
#include "ir/instruction.h"
#include "ir/opcode.h"
#include "ir/sourceloctable.h"
#define accept(p, b, msg) pvm_assert (read_u8 (p) == b, msg);

#include "ir/code.h"
#include "pbcfile/verification/stackemulator.h"
#include "scanner.h"
#include "encoding/utf8.h"
#include "utils/utils.h"
#include "parser.h"
#include "ir/pool.h"
#include "ir/type.h"
#include "utils/common.h"
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <uchar.h>
#include <unicode/umachine.h>

static Pool* build_pool (Parser* parser);
static Header* build_header (Parser* parser);
static char* read_n_bytes (Parser* parser, uint32_t n);
static char* read_utf32_char (Parser* parser);
static char* read_n_utf32_chars (Parser* parser, uint32_t n);
static Function** read_functions (Parser* parser, Pool* pool, uint32_t count);
static Function* read_function (Parser* parser, Pool* pool);
static Instruction* read_instruction (Parser* parser);
static Instruction** read_function_body (Parser* parser, uint32_t code_len);
static SourceLocTable* read_source_loc_table (Parser* parser,
                                              Function* function);

static void check_header (Header* header);
static void check_version (uint8_t patch, uint8_t min, uint8_t maj);

static inline bool is_next_i8 (Parser* parser, int8_t u);
static inline bool is_next_u8 (Parser* parser, uint8_t u);
static inline int8_t read_i8 (Parser* parser);
static inline int16_t read_i16 (Parser* parser);
static inline int32_t read_i32 (Parser* parser);
static inline int64_t read_i64 (Parser* parser);

static inline uint8_t read_u8 (Parser* parser);
static inline uint16_t read_u16 (Parser* parser);
static inline uint32_t read_u32 (Parser* parser);
static inline uint64_t read_u64 (Parser* parser);

Parser* parser_new (const char* input_filename)
{
    Parser* p         = checked_malloc (sizeof (Parser));
    p->input_filename = input_filename;
    p->input          = read_file (input_filename, &p->input_len);
    if (p->input == NULL)
    {
        pvm_panicf ("Could not read file %s: Error: %s (errno %d)",
                    p->input_filename, strerror (errno), errno);
    }

    p->scanner = scanner_new (input_filename, p->input, p->input_len);
    return p;
}

Code* parser_parse (Parser* parser)
{
    Header* file_header = build_header (parser);
    Pool* constant_pool = build_pool (parser);

    uint32_t functions_len = read_u32 (parser);
    Function** functions =
        read_functions (parser, constant_pool, functions_len);

    return code_new (functions, functions_len, file_header, constant_pool);
}

void parser_free (Parser* parser)
{
    return_if_null (parser);
    free (parser->input);
    scanner_free (parser->scanner);
    free (parser);
}

static Pool* build_pool (Parser* parser)
{
    Pool* pool = pool_new ();
    accept (parser, PVM_PARSER_POOL_START,
            "Expected constant pool start byte 0xCB");
    uint32_t len = read_u32 (parser);

    for (uint32_t i = 0; i < len; i++)
    {
        PoolEntry* e;
        uint32_t len = read_u32 (parser);
        int8_t type  = read_i8 (parser);
        pvm_assert (len != 0, "Invalid constant pool entry length 0");
        char* errmsg;
        asprintf (&errmsg, "Invalid constant pool entry type type 0x%02X",
                  type);
        pvm_assert (type == PVM_PARSER_POOL_ENTRY_TYPE_UTF32 ||
                        type == PVM_PARSER_POOL_ENTRY_TYPE_LONG,
                    errmsg);
        free (errmsg);

        if (type == PVM_PARSER_POOL_ENTRY_TYPE_UTF32)
        {
            char* res = read_n_utf32_chars (parser, len);
            e         = pool_entry_new_utf32 (res, len);
        }
        else
        {
            char* len_err_msg;
            asprintf (&len_err_msg,
                      "Invalid constant pool entry length 0x%04X for type long",
                      len);
            pvm_assert (len == 8, len_err_msg);
            free (len_err_msg);
            int64_t data = read_i64 (parser);
            e            = pool_entry_new_long (data);
        }

        pool_add_entry (pool, e);
        char* txt = pool_entry_to_string (e);
        printf ("Entry: %s\n", txt);
        free (txt);
    }

    return pool;
}

static Header* build_header (Parser* parser)
{
    // Get the version
    uint8_t major = read_u8 (parser);
    uint8_t minor = read_u8 (parser);
    uint8_t patch = read_u8 (parser);

    uint8_t srcname_len = read_u8 (parser);
    char* srcname       = read_n_bytes (parser, srcname_len);
    bool hasvendor      = !is_next_u8 (parser, PVM_PARSER_POOL_START);
    char* vendor        = NULL;
    if (hasvendor)
    {
        uint32_t vendor_len = read_u16 (parser);
        vendor              = read_n_bytes (parser, vendor_len);
    }

    Header* header = header_new (major, minor, patch, srcname, vendor);
    check_header (header);
    char* txt = header_to_string (header);
    printf ("Header: %s\n", txt);
    free (txt);
    return header;
}

static char* read_n_bytes (Parser* parser, uint32_t n)
{
    char* s = checked_malloc (sizeof (char) * (n + 1));
    for (uint32_t i = 0; i < n; i++)
    {
        s[i] = read_u8 (parser);
    }

    s[sizeof (char) * n] = 0;
    return s;
}

static char* read_utf32_char (Parser* parser)
{
    char* result = checked_malloc (sizeof (char) * 4);
    uint32_t len;
    result = encode_utf32char (read_i32 (parser), &len);
    return result;
}

static char* read_n_utf32_chars (Parser* parser, uint32_t n)
{
    char* buffer = calloc (4, sizeof (char) * n + 1);
    for (uint32_t i = 0; i < n / 4; i++)
    {
        strcat (buffer, read_utf32_char (parser));
    }

    return buffer;
}

static void check_version (uint8_t patch, uint8_t min, uint8_t maj)
{
    if (maj > pvm_get_major ())
    {
        pvm_panicf (
            "This version of the PVM is only designed to run bytecode with the "
            "version 0x%02X or lower.\nPlease upgrade PVM to run this file.\n",
            pvm_get_major ());
    }

    if (min % 2)
    {
        fprintf (stderr,
                 "WARNING: you are running an unstable version of PVM. Do not "
                 "expect stability from this version.\n");
    }

    pvm_assert (patch < 10, "Invalid patch version\n");
}

static Function** read_functions (Parser* parser, Pool* pool, uint32_t len)
{
    if (len == 0)
    {
        return calloc (0, sizeof (Function*));
    }

    Function** result = malloc (sizeof (Function*) * len);
    for (uint32_t i = 0; i < len; i++)
    {
        result[i] = read_function (parser, pool);
    }

    return result;
}

static Function* read_function (Parser* parser, Pool* pool)
{
    char fmt[] = "Undefined constant pool entry 0x%02X";
    char* out;

    uint32_t sig = read_u32 (parser);
    asprintf (&out, fmt, sig);
    pvm_assert (pool_has_entry (pool, sig), out);
    type_free (type_parse (pool->entries[sig]->value.str));
    free (out);

    uint32_t name = read_u32 (parser);
    asprintf (&out, fmt, name);
    pvm_assert (pool_has_entry (pool, name), out);

    uint32_t code_len  = read_u32 (parser);
    Instruction** body = read_function_body (parser, code_len);

    Function* f       = function_new (body, code_len, name, sig, 0, NULL);
    StackEmulator* se = stack_emulator_new (body, code_len);
    stack_emulator_emulate (se);
    stack_emulator_free (se);
    free (out);
    return f;
}

static Instruction** read_function_body (Parser* parser, uint32_t code_len)
{
    Instruction** result = calloc (code_len, sizeof (Instruction*));
    uint32_t count       = 0;
    for (uint32_t i = 0; i < code_len; i++)
    {
        Instruction* i = read_instruction (parser);
        if (i != NULL)
        {
            result[count++] = i;
        }
    }

    return realloc (result, sizeof (Instruction*) * count);
}

static Instruction* read_instruction (Parser* parser)
{
    uint8_t opcode = read_u8 (parser);
    switch (opcode)
    {
        // Does nothing.
        case OP_NOOP:
        {
            return NULL;
        }

        case OP_IADD:
        case OP_ICONST_1:
        case OP_ICONST_0:
        case OP_DUP:
        case OP_SWAP:
        {
            return instruction_new (opcode, NULL, 0);
        }

        case OP_IPUSH:
        {
            int32_t* args = checked_malloc (sizeof (int32_t));
            args[0]       = read_i32 (parser);
            return instruction_new (opcode, args, 1);
        }

        default:
        {
            pvm_panicf ("Unknown opcode: 0x%02X", opcode);
        }
    }

    return NULL;
}

static void check_header (Header* header)
{
    check_version (header->patch, header->minor, header->major);
    pvm_assert (
        header->sourcename != NULL && strcmp (header->sourcename, "") != 0,
        "Invalid source filename");

    pvm_assert (strcmp (header->vendor, "<none>") == 0 ||
                    strcmp (header->vendor, "") != 0,
                "Invalid compiler vendor string");
}

static inline bool is_next_i8 (Parser* parser, int8_t u)
{
    return scanner_look_i8 (parser->scanner) == u;
}

static inline bool is_next_u8 (Parser* parser, uint8_t u)
{
    return scanner_look_u8 (parser->scanner) == u;
}

static inline int8_t read_i8 (Parser* parser)
{
    return scanner_next_i8 (parser->scanner);
}

static inline int16_t read_i16 (Parser* parser)
{
    return scanner_next_i16 (parser->scanner);
}

static inline int32_t read_i32 (Parser* parser)
{
    return scanner_next_i32 (parser->scanner);
}

static inline int64_t read_i64 (Parser* parser)
{
    return scanner_next_i64 (parser->scanner);
}

static inline uint8_t read_u8 (Parser* parser)
{
    return scanner_next_u8 (parser->scanner);
}

static inline uint16_t read_u16 (Parser* parser)
{
    return scanner_next_u16 (parser->scanner);
}

static inline uint32_t read_u32 (Parser* parser)
{
    return scanner_next_u32 (parser->scanner);
}

static inline uint64_t read_u64 (Parser* parser)
{
    return scanner_next_u64 (parser->scanner);
}