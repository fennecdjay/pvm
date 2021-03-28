// scanner.h: scanner definition
// license information in LICENSE
#ifndef __PVM_SCANNER_H__
#define __PVM_SCANNER_H__
#include <inttypes.h>

typedef struct _Scanner Scanner;

struct _Scanner
{
    const char* input_name;
    int8_t* input;
    uint64_t length;
    uint64_t index;
};

Scanner* scanner_new (const char* input_name, int8_t* input,
                      uint64_t input_length);
int8_t scanner_look_i8 (Scanner* scanner);
int16_t scanner_look_i16 (Scanner* scanner);
int32_t scanner_look_i32 (Scanner* scanner);
int64_t scanner_look_i64 (Scanner* scanner);

uint8_t scanner_look_u8 (Scanner* scanner);
uint16_t scanner_look_u16 (Scanner* scanner);
uint32_t scanner_look_u32 (Scanner* scanner);
uint64_t scanner_look_u64 (Scanner* scanner);

int8_t scanner_next_i8 (Scanner* scanner);
int16_t scanner_next_i16 (Scanner* scanner);
int32_t scanner_next_i32 (Scanner* scanner);
int64_t scanner_next_i64 (Scanner* scanner);

uint8_t scanner_next_u8 (Scanner* scanner);
uint16_t scanner_next_u16 (Scanner* scanner);
uint32_t scanner_next_u32 (Scanner* scanner);
uint64_t scanner_next_u64 (Scanner* scanner);

void scanner_free (Scanner* scanner);

#endif /* __PVM_SCANNER_H__ */