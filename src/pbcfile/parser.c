// parser.c: parser implementation as defined by parser.h
// license information in LICENSE
#include "ir/code.h"
#include "scanner.h"
#include "utf8.h"
#include "utils.h"
#include "parser.h"
#include "pool.h"
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <uchar.h>
#include <unicode/umachine.h>

static int8_t *read_file (const char *fname, uint64_t *flen);
static Pool *build_pool (Parser *parser);
static Header *build_header (Parser *parser);
static char *read_n_bytes (Parser *parser, uint64_t n);
static char *read_utf32_char (Parser *parser);
static char *read_n_utf32_chars (Parser *parser, uint32_t n);
static inline bool is_next_i8 (Parser *parser, int8_t u);
static inline bool is_next_u8 (Parser *parser, uint8_t u);
static inline int8_t read_i8 (Parser *parser);
static inline int16_t read_i16 (Parser *parser);
static inline int32_t read_i32 (Parser *parser);
static inline int64_t read_i64 (Parser *parser);

static inline uint8_t read_u8 (Parser *parser);
static inline uint16_t read_u16 (Parser *parser);
static inline uint32_t read_u32 (Parser *parser);
static inline uint64_t read_u64 (Parser *parser);

Parser *parser_new (const char *input_filename)
{
    Parser *p         = malloc (sizeof (Parser));
    p->input_filename = input_filename;
    p->input          = read_file (input_filename, &p->input_len);
    p->scanner        = scanner_new (input_filename, p->input, p->input_len);
    return p;
}

Code *parser_parse (Parser *parser)
{
    build_header (parser);
    build_pool (parser);
    return NULL;
}

void parser_free (Parser *parser)
{
    return_if_null (parser);
    free (parser->input);
    scanner_free (parser->scanner);
    free (parser);
}

static int8_t *read_file (const char *fname, uint64_t *flen)
{
    FILE *fileptr;
    int8_t *buffer;
    uint64_t len;

    fileptr = fopen (fname, "rb");
    if (fileptr == NULL)
    {
        pvm_panicf ("%s: %s (errno %d)", fname, strerror (errno), errno);
    }

    fseek (fileptr, 0, SEEK_END);
    len = ftell (fileptr);
    rewind (fileptr);

    buffer = (int8_t *) malloc (len * sizeof (int8_t));
    fread (buffer, len, 1, fileptr);
    fclose (fileptr);
    *flen = len;
    return buffer;
}

static Pool *build_pool (Parser *parser)
{
    Pool *pool = pool_new ();
    while (is_next_i8 (parser, PVM_PARSER_POOL_ENTRY_BEGIN))
    {
        parser->scanner->index++;
        PoolEntry *e;
        uint32_t len = read_u32 (parser);
        int8_t type  = read_i8 (parser);
        pvm_assert (len != 0, "Invalid constant pool entry length 0");
        char *errmsg;
        asprintf (&errmsg, "Invalid constant pool entry type type 0x%02X",
                  type);
        pvm_assert (type == PVM_PARSER_POOL_ENTRY_TYPE_UTF32 ||
                        type == PVM_PARSER_POOL_ENTRY_TYPE_LONG,
                    errmsg);


        if (type == PVM_PARSER_POOL_ENTRY_TYPE_UTF32)
        {
            char *res = read_n_utf32_chars (parser, len);
            e         = pool_entry_new_utf32 (res, len);
        }
        else
        {
            char *len_err_msg;
            asprintf (&len_err_msg,
                      "Invalid constant pool entry length 0x%04X for type long",
                      len);
            pvm_assert (len == 8, len_err_msg);
            int64_t data = read_i64 (parser);
            e            = pool_entry_new_long (data);
        }

        pool_add_entry (pool, e);
        // printf ("%s\n", pool_entry_to_string (e));
    }

    return pool;
}

static Header *build_header (Parser *parser)
{
    // Get the version
    uint8_t major = read_u8 (parser);
    uint8_t minor = read_u8 (parser);
    uint8_t patch = read_u8 (parser);

    uint8_t srcname_len = read_u8 (parser);
    char *srcname       = read_n_bytes (parser, srcname_len);
    bool hasvendor      = !is_next_u8 (parser, PVM_PARSER_POOL_START);
    char *vendor        = NULL;
    if (hasvendor)
    {
        uint16_t vendor_len = read_u16 (parser);
        vendor              = read_n_bytes (parser, vendor_len);
    }

    Header *header = header_new (major, minor, patch, srcname, vendor);
    printf ("%s\n", header_to_string (header));
    return header;
}

static char *read_n_bytes (Parser *parser, uint64_t n)
{
    char *s = malloc (sizeof (char) * (n + 1));
    for (uint64_t i = 0; i < n; i++)
    {
        s[i] = read_u8 (parser);
    }

    s[sizeof (char) * n] = 0;
    return s;
}

static char *read_utf32_char (Parser *parser)
{
    char *result = malloc (sizeof (char) * 4);
    uint32_t len;
    result = encode_utf8char (read_i32 (parser), &len);
    printf ("r: %s\n", result);
    return result;
}

static char *read_n_utf32_chars (Parser *parser, uint32_t n)
{
    char *buffer = calloc (4, sizeof (char) * n + 1);
    for (uint32_t i = 0; i < n / 4; i++)
    {
        strcat (buffer, read_utf32_char (parser));
    }

    return buffer;
}

static inline bool is_next_i8 (Parser *parser, int8_t u)
{
    return scanner_look_i8 (parser->scanner) == u;
}

static inline bool is_next_u8 (Parser *parser, uint8_t u)
{
    return scanner_look_u8 (parser->scanner) == u;
}

static inline int8_t read_i8 (Parser *parser)
{
    return scanner_next_i8 (parser->scanner);
}

static inline int16_t read_i16 (Parser *parser)
{
    return scanner_next_i16 (parser->scanner);
}

static inline int32_t read_i32 (Parser *parser)
{
    return scanner_next_i32 (parser->scanner);
}

static inline int64_t read_i64 (Parser *parser)
{
    return scanner_next_i64 (parser->scanner);
}

static inline uint8_t read_u8 (Parser *parser)
{
    return scanner_next_u8 (parser->scanner);
}

static inline uint16_t read_u16 (Parser *parser)
{
    return scanner_next_u16 (parser->scanner);
}

static inline uint32_t read_u32 (Parser *parser)
{
    return scanner_next_u32 (parser->scanner);
}

static inline uint64_t read_u64 (Parser *parser)
{
    return scanner_next_u64 (parser->scanner);
}