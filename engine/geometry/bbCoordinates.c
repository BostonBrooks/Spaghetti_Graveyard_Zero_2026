#include "engine/data/bbIncludes.c"
#include "engine/data/bbConstants.h"

sfVector2f bbScreenPoints_getV2f(bbScreenPoints SP)
{
    sfVector2f return_value;
    return_value.x = SP.x / SCREEN_PPPf;
    return_value.y = SP.y / SCREEN_PPPf;

    return return_value;
}

bbScreenPoints pixel_getScreenPoints(I32 x, I32 y)
{
    bbScreenPoints SP;
    SP.x = x * SCREEN_PPP;
    SP.y = y * SCREEN_PPP;

    return SP;
}

I32 bbScreenPoints_inScreenPointsRect(bbScreenPoints SP,
                                      bbScreenPointsRect rect){

    if (SP.x < rect.left || SP.y < rect.top) return 0;

    I32 right = rect.left + rect.width;
    I32 bottom = rect.top + rect.height;

    if (SP.x > right || SP.y > bottom) return 0;

    return 1;
}