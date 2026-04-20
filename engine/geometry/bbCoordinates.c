#include "engine/data/bbIncludes.c"
#include "engine/data/bbConstants.h"
#include "engine/geometry/bbCoordinates.h"

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




bbMilliCoords bbMapCoords_getMilliCoords(bbMapCoords MC){
    bbMilliCoords mC;
    mC.i = MC.i * MILLS_PER_POINT;
    mC.j = MC.j * MILLS_PER_POINT;

    // unused for now
    mC.k = MC.k;
    return mC;
}

bbMapCoords bbMilliCoords_getMapCoords(bbMilliCoords mC){
    bbMapCoords MC;
    MC.i = bbArith_div(mC.i, MILLS_PER_POINT);
    MC.j = bbArith_div(mC.j, MILLS_PER_POINT);

    // unused for now
    MC.k = mC.k;
    return MC;
}

bbTileCoords bbMapCoords_getTileCoords(bbMapCoords MC){
    bbTileCoords TC;
    TC.i = bbArith_div(MC.i, POINTS_PER_TILE);
    TC.j = bbArith_div(MC.j, POINTS_PER_TILE);
    TC.k = MC.k;
    return TC;
}

bbSquareCoords bbMapCoords_getSquareCoords(bbMapCoords MC){
    bbSquareCoords SC;
    SC.i = bbArith_div(MC.i, POINTS_PER_SQUARE);
    SC.j = bbArith_div(MC.j, POINTS_PER_SQUARE);
    SC.k = MC.k;
    return SC;
}

bbSquareCoords bbTileCoords_getSquareCoords(bbTileCoords TC){
    bbSquareCoords SC;
    SC.i = bbArith_div(TC.i, POINTS_PER_TILE);
    SC.j = bbArith_div(TC.j, POINTS_PER_TILE);
    SC.k = TC.k;
    return SC;
}

/// get a vector from A to B, as seen on the 2d isometric projection
bbScreenPoints bbMapCoords_getScreenPoints_relative (bbMapCoords A,
                                                     bbMapCoords B){
    bbMapCoords delta;
    delta.i = B.i - A.i;
    delta.j = B.j - A.j;
    delta.k = B.k - A.k;

    bbScreenPoints SP;
    SP.x = delta.i + delta.j;
    SP.y = bbArith_div((delta.i - delta.j), 2) - delta.k;

    return SP;
}

bbFloat3D bbFloat3D_normalise (bbFloat3D foo){
    float abs = sqrt (foo.i * foo.i + foo.j * foo.j + foo.k * foo.k );
    foo.i /= abs;
    foo.j /= abs;
    foo.k /= abs;

    return foo;
}


bbMapCoords bbSquareCoords_getMapCoords(bbSquareCoords SC){
	bbMapCoords MC;
	MC.i = SC.i * POINTS_PER_SQUARE;
	MC.j = SC.j * POINTS_PER_SQUARE;
	MC.k = 0;

	return MC;
}

bbMilliCoords bbMilliCoords_interpolate(bbMilliCoords MC1, bbMilliCoords MC2, I64 T0, I64 T1, I64 T2)
{

    if (T2 == T0)
    {
        return MC2;
    }

    bbMilliCoords delta;
    delta.i = MC2.i - MC1.i;
    delta.j = MC2.j - MC1.j;
    delta.k = MC2.k - MC1.k;

    delta.i *= (T1 - T0);
    delta.j *= (T1 - T0);
    delta.k *= (T1 - T0);

    delta.i /= (T2 - T0);
    delta.j /= (T2 - T0);
    delta.k /= (T2 - T0);

    delta.i += MC1.i;
    delta.j += MC1.j;
    delta.k += MC1.k;

    return delta;

}

bbMapCoords bbMapCoords_interpolate(bbMapCoords MC1, bbMapCoords MC2, I64 T0, I64 T1, I64 T2)
{

    if (T2 == T0)
    {
        return MC2;
    }

    bbMapCoords delta;
    delta.i = MC2.i - MC1.i;
    delta.j = MC2.j - MC1.j;
    delta.k = MC2.k - MC1.k;

    delta.i *= (T1 - T0);
    delta.j *= (T1 - T0);
    delta.k *= (T1 - T0);

    delta.i /= (T2 - T0);
    delta.j /= (T2 - T0);
    delta.k /= (T2 - T0);

    delta.i += MC1.i;
    delta.j += MC1.j;
    delta.k += MC1.k;

    return delta;

}