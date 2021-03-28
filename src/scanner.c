// scanner.c: scanner implementation as defined by scanner.h
// license information in LICENSE
#include "scanner.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "config.h"

static void scanner_assert_remaining (Scanner* scanner, uint8_t remaining)
{
    if ((scanner->length - scanner->index) < remaining)
    {
        pvm_errprintf ("Ran out of bytes: %d expected, got %d", remaining,
                       scanner->length - scanner->index);
    }
}

Scanner* scanner_new (const char* input_name, int8_t* input,
                      uint64_t input_length)
{
    Scanner* scanner    = malloc (sizeof (Scanner));
    scanner->index      = 0;
    scanner->input_name = input_name;
    scanner->input      = input;
    scanner->length     = input_length;
    return scanner;
}

int8_t scanner_look_i8 (Scanner* scanner)
{
    return scanner->input[scanner->index];
}

int8_t scanner_next_i8 (Scanner* scanner)
{
    scanner_assert_remaining (scanner, 1);
    return scanner->input[scanner->index++];
}

int16_t scanner_look_i16 (Scanner* scanner)
{
    scanner_assert_remaining (scanner, 1);
    int16_t result;
    int8_t next2[] = {
        scanner->input[scanner->index],
        scanner->input[scanner->index + 1],
    };

    memcpy (&result, next2, sizeof (int8_t) * 2);
#if LILENDIAN
    return swap_i16 (result);
#else
    return result;
#endif
}

int16_t scanner_next_i16 (Scanner* scanner)
{
    scanner_assert_remaining (scanner, 1);
    int16_t result;
    int8_t next2[] = {
        scanner->input[scanner->index++],
        scanner->input[scanner->index++],
    };

    memcpy (&result, next2, sizeof (int8_t) * 2);

#if LILENDIAN
    return swap_i16 (result);
#else
    return result;
#endif
}

int32_t scanner_look_i32 (Scanner* scanner)
{
    scanner_assert_remaining (scanner, 3);
    int32_t result;
    int8_t next4[] = {
        scanner->input[scanner->index],
        scanner->input[scanner->index + 1],
        scanner->input[scanner->index + 2],
        scanner->input[scanner->index + 3],
    };

    memcpy (&result, next4, sizeof (int8_t) * 4);

#if LILENDIAN
    return swap_i32 (result);
#else
    return result;
#endif
}

int32_t scanner_next_i32 (Scanner* scanner)
{
    scanner_assert_remaining (scanner, 3);
    int32_t result;
    int8_t next4[] = {
        scanner->input[scanner->index++],
        scanner->input[scanner->index++],
        scanner->input[scanner->index++],
        scanner->input[scanner->index++],
    };

    memcpy (&result, next4, sizeof (int8_t) * 4);

#if LILENDIAN
    return swap_i32 (result);
#else
    return result;
#endif
}

int64_t scanner_look_i64 (Scanner* scanner)
{
    scanner_assert_remaining (scanner, 7);
    int64_t result;
    int8_t next8[] = {
        scanner->input[scanner->index],     scanner->input[scanner->index + 1],
        scanner->input[scanner->index + 2], scanner->input[scanner->index + 3],
        scanner->input[scanner->index + 4], scanner->input[scanner->index + 5],
        scanner->input[scanner->index + 6], scanner->input[scanner->index + 7],
    };

    memcpy (&result, next8, sizeof (int8_t) * 8);

#if LILENDIAN
    return swap_i64 (result);
#else
    return result;
#endif
}

int64_t scanner_next_i64 (Scanner* scanner)
{
    scanner_assert_remaining (scanner, 7);
    int64_t result;
    int8_t next8[] = {
        scanner->input[scanner->index++], scanner->input[scanner->index++],
        scanner->input[scanner->index++], scanner->input[scanner->index++],
        scanner->input[scanner->index++], scanner->input[scanner->index++],
        scanner->input[scanner->index++], scanner->input[scanner->index++],
    };

    memcpy (&result, next8, sizeof (int8_t) * 8);

#if LILENDIAN
    return swap_i64 (result);
#else
    return result;
#endif
}

uint8_t scanner_look_u8 (Scanner* scanner)
{
    return (uint8_t) scanner->input[scanner->index];
}

uint16_t scanner_look_u16 (Scanner* scanner)
{
    return (uint64_t) scanner_look_i16 (scanner);
}

uint32_t scanner_look_u32 (Scanner* scanner)
{
    return (uint32_t) scanner_look_i32 (scanner);
}

uint64_t scanner_look_u64 (Scanner* scanner)
{
    return (uint64_t) scanner_look_i64 (scanner);
}

uint8_t scanner_next_u8 (Scanner* scanner)
{
    scanner_assert_remaining (scanner, 1);
    return (uint8_t) scanner->input[scanner->index++];
}

uint16_t scanner_next_u16 (Scanner* scanner)
{
    return (uint64_t) scanner_next_i16 (scanner);
}

uint32_t scanner_next_u32 (Scanner* scanner)
{
    return (uint32_t) scanner_next_i32 (scanner);
}

uint64_t scanner_next_u64 (Scanner* scanner)
{
    return (uint64_t) scanner_next_i64 (scanner);
}

void scanner_free (Scanner* scanner)
{
    return_if_null (scanner);
    free (scanner);
}