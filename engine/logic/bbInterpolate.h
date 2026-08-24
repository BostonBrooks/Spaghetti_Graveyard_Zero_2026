#ifndef  BB_INTERPOLATE
#define  BB_INTERPOLATE
#include "bbFlag.h"
#include "bbIntTypes.h"

typedef struct
{
    U32 length;
    I64 denominator;

    I64 numbers[];
} bbInterpolate;

bbFlag bbInterpolate_new(bbInterpolate** interp, char* filename);

bbFlag bbInterpolate_interp(bbInterpolate* interp, I64* y, I64 x);
#endif BB_INTERPOLATE