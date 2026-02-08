#include "engine/data/bbIncludes.c"
#include "engine/data/bbConstants.h"

sfVector2f bbScreenPoints_getV2f(bbScreenPoints SP)
{
    sfVector2f return_value;
    return_value.x = SP.x / SCREEN_PPPf;
    return_value.y = SP.y / SCREEN_PPPf;

    return return_value;
}