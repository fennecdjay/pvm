// byteUtils.h: Bytecode utilities
// written by apachejuice
// license information in LICENSE
#ifndef __pvm_byteutils_h__
#define __pvm_byteutils_h__
#include <inttypes.h>

int16_t byte_utils_to_i16 (int8_t* bytes);
int32_t byte_utils_to_i32 (int8_t* bytes);
int64_t byte_utils_to_i64 (int8_t* bytes);
uint16_t byte_utils_to_u16 (int8_t* bytes);
uint32_t byte_utils_to_u32 (int8_t* bytes);
uint64_t byte_utils_to_u64 (int8_t* bytes);
float byte_utils_to_f32 (int8_t* bytes);
double byte_utils_to_f64 (int8_t* bytes);

int16_t byte_utils_sw_i16 (int16_t n);
int32_t byte_utils_sw_i32 (int32_t n);
int64_t byte_utils_sw_i64 (int64_t n);
uint16_t byte_utils_sw_u16 (uint16_t n);
uint32_t byte_utils_sw_u32 (uint32_t n);
uint64_t byte_utils_sw_u64 (uint64_t n);

#endif
