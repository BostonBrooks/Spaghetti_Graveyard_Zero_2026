/// This header file contains wrappers for int types

#ifndef BB_INTTYPES_H
#define BB_INTTYPES_H

#include <inttypes.h>
#include <stdbool.h>

#define I32    int32_t
#define I64    int64_t
#define U8     uint8_t
#define U16    uint16_t
#define U32    uint32_t
#define U64    uint64_t

#define sfUint8 uint8_t

#define I32_MAX 2147483647l
#define I32_MIN -2147483647l

#ifdef CSFML3
#define sfBool   bool
#define sfTrue   true
#define sfFalse  false
#endif

typedef struct
{
    I32 x;
    I32 y;
} I32x2;

#endif //BB_INTTYPES_H