/**
 * @file
 * @brief We assume built in integer arithmetic rounds to zero
 * Define new arithmetic that rounds toward -infinity
 */

#ifndef BB_ARITHMETIC_H
#define BB_ARITHMETIC_H

#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbTerminal.h"

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

#endif //BB_ARITHMETIC_H