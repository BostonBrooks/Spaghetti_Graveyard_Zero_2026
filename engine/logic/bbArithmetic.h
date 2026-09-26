/**
 * @file
 * @brief We assume built in integer arithmetic rounds to zero
 * Define new arithmetic that rounds toward -infinity
 */

#ifndef BB_ARITHMETIC_H
#define BB_ARITHMETIC_H



#include "engine/logic/bbIntTypes.h"


#include "engine/logic/bbTerminal.h"

static U32 bbArith_sqrt(U32 n)
{
    if (n <= 1) return n;

    U32 x = n;
    U32 y = (n + 1) / 2;

    while (y < x)
    {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

/// The larger of two integers
static I32 bbArith_max (I32 x, I32 y){
    return x > y ? x : y;
}

/// The smaller of two integers
static I32 bbArith_min (I32 x, I32 y){
    return x > y ? y : x;
}

/// divide and round down
static I32 bbArith_div (I32 x, I32 y){
    bbAssert(y != 0, "divide by zero\n");
    if ((x < 0 &&  y > 0) || (x > 0 && y < 0) && x%y != 0)
        return x/y - 1;
    return x/y;
}

/// like the % operator, but always positive
static I32 bbArith_mod (I32 x, I32 y){
    return x - y* bbArith_div(x, y);
}

/// divide and round up
static I32 bbArith_ceiling (I32 x, I32 y){
    return bbArith_div(x, y) + (x%y != 0);
}

/// divide and round down IE bbArith_div
static I32 bbArith_floor (I32 x, I32 y){
    bbAssert(y != 0, "divide by zero\n");
    if ((x < 0 &&  y > 0) || (x > 0 && y < 0) && x%y != 0)
        return x/y - 1;
    return x/y;
}

/// round x up to the nearest multiple of y
static I32 bbArith_roundUp (I32 x, I32 y){
    return x >= 0 ? ((x + y - 1) / y) * y : (x / y) * y;
}

/// round x down to the nearest multiple of y
static I32 bbArith_roundDown (I32 x, I32 y){
    return x >= 0 ? (x / y) * y : ((x - y + 1) / y) * y;
}

static U64 bbArith64_sqrt(U64 n)
{
    if (n <= 1) return n;

    U64 x = n;
    U64 y = (n + 1) / 2;

    while (y < x)
    {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}


static U64 bbArith64_sqrt2(U64 n)
{
    U64 res = 0;
    U64 bit = 1LLU << 62;

    while (bit > n)
    {
        bit >>= 2;
    }

    while (bit != 0)
    {
        if (n >= res + bit)
        {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else
        {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

static I64 bbArith64_max (I64 x, I64 y){
    return x > y ? x : y;
}
static I64 bbArith64_min (I64 x, I64 y){
    return x > y ? y : x;
}
static I64 bbArith64_div (I64 x, I64 y){
    bbAssert(y != 0, "divide by zero\n");
    if ((x < 0 &&  y > 0) || (x > 0 && y < 0) && x%y != 0)
        return x/y - 1;
    return x/y;
}
static I64 bbArith64_mod (I64 x, I64 y){
    return x - y* bbArith64_div(x, y);
}
static I64 bbArith64_ceiling (I64 x, I64 y){
    return bbArith64_div(x, y) + (x%y != 0);
}
static I64 bbArith64_floor (I64 x, I64 y){
    bbAssert(y != 0, "divide by zero\n");
    if ((x < 0 &&  y > 0) || (x > 0 && y < 0) && x%y != 0)
        return x/y - 1;
    return x/y;
}
static I64 bbArith64_roundUp (I64 x, I64 y){
    return x >= 0 ? ((x + y - 1) / y) * y : (x / y) * y;
}

static I64 bbArith64_roundDown (I64 x, I64 y){
    return x >= 0 ? (x / y) * y : ((x - y + 1) / y) * y;
}

static U64 bbArith64_setBit(U64* bitmap, char bit)
{
    U64 mask = 1LLU << bit;
    *bitmap |= mask;
    return *bitmap;
}

static U64 bbArith64_clearBit(U64* bitmap, char bit)
{
    U64 mask = 1LLU << bit;
    *bitmap &= ~mask;
    return *bitmap;
}

static U64 bbArith64_hash(U64 old_hash) {
    U64 new_hash = old_hash + 0x9e3779b97f4a7c15ULL;

    // 2. Apply a 64-bit avalanche hash mixer (bit-shifts and prime multipliers)

    new_hash  = (new_hash  ^ (new_hash  >> 30)) * 0xbf58476d1ce4e5b9ULL;
    new_hash  = (new_hash  ^ (new_hash  >> 27)) * 0x94d049bb133111ebULL;
    return new_hash  ^ (new_hash  >> 31);
}

static U64 bbArith64_hashIndex(U64 old_hash, I64 index) {
    U64 new_hash = index + old_hash + 0x9e3779b97f4a7c15ULL;

    // 2. Apply a 64-bit avalanche hash mixer (bit-shifts and prime multipliers)

    new_hash  = (new_hash  ^ (new_hash  >> 30)) * 0xbf58476d1ce4e5b9ULL;
    new_hash  = (new_hash  ^ (new_hash  >> 27)) * 0x94d049bb133111ebULL;
    return new_hash  ^ (new_hash  >> 31);
}


#endif //BB_ARITHMETIC_H