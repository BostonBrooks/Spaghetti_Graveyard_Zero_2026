#include <stdio.h>

#include "engine/logic/bbInterpolate.h"

int main (void)
{
    bbInterpolate* interp;

    bbInterpolate_new(&interp, "test_interpolate.csv");



    for (I32 i=0; i<100; i++)
    {
        I64 y;
        bbInterpolate_interp(interp, &y, i-10);

        printf("%d,%d\n", i-10, y);
    }

}
