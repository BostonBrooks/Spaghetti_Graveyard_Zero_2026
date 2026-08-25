#include "engine/logic/bbInterpolate.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"
#define DEBUG_INTERPOLATE

bbFlag bbInterpolate_new(bbInterpolate** interp, char* filename)
{
    FILE* file = fopen(filename, "r");

    U32 length;

    fscanf(file, "%" SCNu32 ",", &length);

    I64 divisor;
    fscanf(file, "%" SCNi64 "\n", &divisor);

    bbInterpolate* interpolate = malloc(
        sizeof(bbInterpolate) + 2 * sizeof(I64) * length);

    interpolate->length = length;
    interpolate->denominator = divisor;

    for (U32 i=0; i<length; i++)
    {
        fscanf(file, "%" SCNi64 ",%" SCNi64 "\n",&interpolate->numbers[2*i], &interpolate->numbers[2*i+1]);
    }


#ifdef DEBUG_INTERPOLATE
    printf("length = %d, divisor = %d",
        interpolate->length,
        interpolate->denominator);

    for (U32 i=0; i<length; i++)
    {
        printf("%lld,%lld\n",interpolate->numbers[2*i], interpolate->numbers[2*i+1]);
    }
    printf("\n");

#endif
    *interp = interpolate;


    fclose(file);

    return bbSuccess;
}


bbFlag bbInterpolate_interp(bbInterpolate* interp, I64* y, I64 x)
{
    if (x < interp->numbers[0])
    {
        *y = interp->numbers[1];
        return bbSuccess;
    }

    for (U32 i=0; i<interp->length-1; i++)
    {
        if (x>interp->numbers[i*2+2]) continue;

        I64 x_left = interp->numbers[i*2];
        I64 y_left = interp->numbers[i*2+1];

        if (x==x_left)
        {
            *y = y_left;
            return bbSuccess;
        }

        I64 x_right = interp->numbers[i*2+2];
        I64 y_right = interp->numbers[i*2+3];

        I64 delta_y = y_right - y_left;
        delta_y *= (x-x_left);
        delta_y /= (x_right-x_left);
        *y = y_left + delta_y;
        return bbSuccess;

    }
    *y = interp->numbers[2*(interp->length-1)+1];
    return bbSuccess;

}