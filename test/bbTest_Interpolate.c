#include <stdio.h>

#include "engine/logic/bbArithmetic.h"
#include "engine/logic/bbInterpolate.h"

int main (void)
{
    U64 sqrt = bbArith64_sqrt(1000000);
    U64 sqrt2 = bbArith64_sqrt2(1000000);

    printf("sqrt = %llu,sqrt2 = %llu\n", sqrt, sqrt2);

    bbInterpolate* interp;

    bbInterpolate_new(&interp, "test_interpolate.csv");


    for (I32 i=0; i<100; i++)
    {
        I64 y;
        bbInterpolate_interp(interp, &y, i-10);

        printf("%d,%d\n", i-10, y);
    }

}
