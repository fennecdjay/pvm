// byteUtils.cc: Bytecode utilities
// written by apachejuice
// license information in LICENSE
#include <string.h>
#include "pbc/byteUtils.h"
#include "config.h"
#include "utils/utils.h"

int16_t byte_utils_sw_i16 (int16_t n)
{
    return (n << 8) | ((n >> 8) & 0xFF);
}

int32_t byte_utils_sw_i32 (int32_t n)
{
    n = ((n << 8) & 0xFF00FF00) | ((n >> 8) & 0xFF00FF);
    return (n << 16) | ((n >> 16) & 0xFFFF);
}

int64_t byte_utils_sw_i64 (int64_t n)
{
    n = ((n << 8) & 0xFF00FF00FF00FF00ULL) | ((n >> 8) & 0x00FF00FF00FF00FFULL);
    n = ((n << 16) & 0xFFFF0000FFFF0000ULL) |
        ((n >> 16) & 0x0000FFFF0000FFFFULL);
    return (n << 32) | ((n >> 32) & 0xFFFFFFFFULL);
}

uint16_t byte_utils_sw_u16 (uint16_t n)
{
    return (n << 8) | (n >> 8);
}

uint32_t byte_utils_sw_u32 (uint32_t n)
{
    n = ((n << 8) & 0xFF00FF00) | ((n >> 8) & 0xFF00FF);
    return (n << 16) | (n >> 16);
}

uint64_t byte_utils_sw_u64 (uint64_t n)
{
    n = ((n << 8) & 0xFF00FF00FF00FF00ULL) | ((n >> 8) & 0x00FF00FF00FF00FFULL);
    n = ((n << 16) & 0xFFFF0000FFFF0000ULL) |
        ((n >> 16) & 0x0000FFFF0000FFFFULL);
    return (n << 32) | (n >> 32);
}

int16_t byte_utils_to_i16 (int8_t* bytes)
{
    int8_t arr[] = {
        bytes[0],
        bytes[1],
    };
    int16_t result;
    memcpy (&result, arr, sizeof (int8_t) * 2);

#if IS_LIL_ENDIAN_UWU
    return byte_utils_sw_i16 (result);
#else
    return result;
#endif
}

int32_t byte_utils_to_i32 (int8_t* bytes)
{
    int8_t arr[] = {
        bytes[0],
        bytes[1],
        bytes[2],
        bytes[3],
    };
    int32_t result;
    memcpy (&result, arr, sizeof (int8_t) * 4);

#if IS_LIL_ENDIAN_UWU
    return byte_utils_sw_i32 (result);
#else
    return result;
#endif
}

int64_t byte_utils_to_i64 (int8_t* bytes)
{
    int8_t arr[] = {
        bytes[0], bytes[1], bytes[2], bytes[3],
        bytes[4], bytes[5], bytes[6], bytes[7],
    };
    int64_t result;
    memcpy (&result, arr, sizeof (int8_t) * 8);

#if IS_LIL_ENDIAN_UWU
    return byte_utils_sw_i64 (result);
#else
    return result;
#endif
}

uint16_t byte_utils_to_u16 (int8_t* bytes)
{
    int8_t arr[] = {
        bytes[0],
        bytes[1],
    };
    int16_t result;
    memcpy (&result, arr, sizeof (int8_t) * 2);

#if IS_LIL_ENDIAN_UWU
    return byte_utils_sw_u16 (result);
#else
    return result;
#endif
}

uint32_t byte_utils_to_u32 (int8_t* bytes)
{
    int8_t arr[] = {
        bytes[0],
        bytes[1],
        bytes[2],
        bytes[3],
    };
    int32_t result;
    memcpy (&result, arr, sizeof (int8_t) * 4);

#if IS_LIL_ENDIAN_UWU
    return byte_utils_sw_u32 (result);
#else
    return result;
#endif
}

uint64_t byte_utils_to_u64 (int8_t* bytes)
{
    int8_t arr[] = {
        bytes[0], bytes[1], bytes[2], bytes[3],
        bytes[4], bytes[5], bytes[6], bytes[7],
    };
    uint64_t result;
    memcpy (&result, arr, sizeof (int8_t) * 8);

#if IS_LIL_ENDIAN_UWU
    return byte_utils_sw_u64 (result);
#else
    return result;
#endif
}

float byte_utils_to_f32 (int8_t* bytes)
{
    float f;
    int8_t* f_ptr = (int8_t*) &f;
    f_ptr[3]      = bytes[0];
    f_ptr[2]      = bytes[1];
    f_ptr[1]      = bytes[2];
    f_ptr[0]      = bytes[3];
    return f;
}

double byte_utils_to_f64 (int8_t* bytes)
{
    double d;
    int8_t* d_ptr = (int8_t*) &d;
    d_ptr[7]      = bytes[0];
    d_ptr[6]      = bytes[1];
    d_ptr[5]      = bytes[2];
    d_ptr[4]      = bytes[3];
    d_ptr[3]      = bytes[4];
    d_ptr[2]      = bytes[5];
    d_ptr[1]      = bytes[6];
    d_ptr[0]      = bytes[7];
    return d;
}
