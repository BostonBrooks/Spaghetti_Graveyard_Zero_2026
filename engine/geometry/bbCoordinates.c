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
// float projectile_height (float start_time,
//                        float end_time,
//                        float start_height,
//                        float end_height,
//                        float gravity,
//                        float current_time) {
//     float dt = end_time - start_time;
//
//     if (dt <= 0.0f) return end_height;
//     float u = (current_time - start_time) / dt;
//     if (u<=0.0f) return start_height;
//     if (u>=1.0f) return end_height;
//     return start_height + (end_height - start_height) * u + gravity * dt * dt * (u*u-u);
// }

//g approx -0.1 G approx 1000
bbMapCoords bbMapCoords_interpolateGravity(bbMapCoords MC1, bbMapCoords MC2, I64 G, I64 T0, I64 T1, I64 T2)
{


    if (T2 == T0)
    {
        return MC2;
    }

    bbMapCoords MC3;

    I64 delta_t = T2-T0;
    I64 delta_t1 = T1-T0;
    I64 delta_t2 = T2-T1;

    I64 start_i = MC1.i;
    I32 end_i = MC2.i;

    I64 start_j = MC1.j;
    I32 end_j = MC2.j;

    I64 start_k = MC1.k;
    I64 end_k = MC2.k;

//TODO bbArith64_div()
    MC3.i = start_i + (end_i - start_i) * delta_t1 / delta_t;
    MC3.j = start_j + (end_j - start_j) * delta_t1 / delta_t;
    MC3.k = start_k + (end_k - start_k) * delta_t1 / delta_t;

    MC3.k +=  G * delta_t1 * delta_t2 / 16384;

    return MC3;
}

bbFloat3D bbMapCoords_getVelocity(bbMapCoords MC1, bbMapCoords MC2, I64 G, I64 T0, I64 T1, I64 T2) {


    float delta_t = T2-T0;
    float delta_i = MC2.i - MC1.i;
    float delta_j = MC2.j - MC1.j;
    float delta_k = MC2.k - MC1.k;

    bbFloat3D linear;
    linear.i = delta_i /delta_t;
    linear.j = delta_j /delta_t;
    linear.k = delta_k /delta_t;

    linear.k += (float)(-2*T1+T2+T0) * G / 16384;

    return linear;

}

bbFloat3D bbFloat3D_rect_to_polar (bbFloat3D rect)
{
    float r = sqrt(rect.i*rect.i + rect.j*rect.j + rect.k*rect.k);
    float direction = atan2(rect.i,rect.j);
    float polar = acos(rect.k / r);

    bbFloat3D spherical;
    spherical.i = r;
    spherical.j = direction;
    spherical.k = polar;

    return spherical;

}


bbFlag bbMapCoords_withinCircle(bbMapCoords Point, bbMapCoords Centre, I32 radius)
{
    I32 delta_i = Point.i - Centre.i;
    I32 delta_j = Point.j - Centre.j;
    I32 delta_k = Point.k - Centre.k;

    I32 delta_x = delta_i + delta_j;
    I32 delta_y = bbArith_div((delta_i - delta_j), 2) - delta_k;

    if (delta_x*delta_x + delta_y*delta_y > radius*radius) return bbNone;


    return bbSuccess;

}

///Is the Point within a rect centred at Centre?
bbFlag bbMapCoords_withinRect(bbMapCoords Point, bbMapCoords Centre, bbGameWorldRect rect)
{
    I32 delta_i = Point.i - Centre.i;
    I32 delta_j = Point.j - Centre.j;
    I32 delta_k = Point.k - Centre.k;

    I32 delta_x = delta_i + delta_j;
    I32 delta_y = delta_k - bbArith_div((delta_i - delta_j), 2);

    I32 top = rect.top;                   //>0
    I32 bottom = rect.top - rect.height;  //<0
    I32 left = rect.left;                 //>0
    I32 right = rect.left - rect.width;   //<0

    //if delta_x, delta_y == 0
    //then top > delta_y > bottom
    //and left > delta_x > right
    //so then we return bbSuccess

    if (top > delta_y && delta_y > bottom && left > delta_x && delta_x > right) return bbSuccess;

    return bbNone;

}