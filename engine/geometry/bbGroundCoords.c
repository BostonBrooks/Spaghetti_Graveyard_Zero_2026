#include "engine/geometry/bbGroundCoords.h"

#include "engine/data/bbConstants.h"
#include "engine/viewport/bbViewport.h"


//bbMapCoords bbScreenCoords_getMapCoords(bbScreenPoints SP);
//I32 bbTileCoords_getElevation(bbGroundSurface* surface, bbTileCoords coords);
//I32 bbMapCoords_getElevation(bbGroundSurface* ground_surface, bbMapCoords coords);
//static I32 bbArith_div (I32 x, I32 y);
//bbSquareCoords bbMapCoords_getSquareCoords(bbMapCoords MC);

bbFlag matrix_multiply (float A[4][4], float B[4][4], float C[4][4]){

    // A B = C

    int i, j, k;

    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            C[i][j] = 0;
        }
    }

    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            for (k = 0; k < 4; k++){
                C[i][j] += A[i][k]* B[k][j];
            }
        }
    }

    return bbSuccess;
}

float signed_area (bbViewportPoints p1, bbViewportPoints p2, bbViewportPoints p3) {
    //positive is clockwise

    float x1, y1, x2, y2;

    x1 = p2.x - p1.x;
    y1 = p2.y - p1.y;

    x2 = p3.x - p2.x;
    y2 = p3.y - p2.y;

    float area = (x1 * y2 - x2 * y1) / 2;

    return area;
}

I32 bbViewportPoints_withinTriangle_screen (bbViewportPoints point,
    bbViewportPoints vertex1, bbViewportPoints vertex2, bbViewportPoints vertex3){

    float area = signed_area (vertex1, vertex2, vertex3);

    if (area == 0) {

        return 0;
    }

    //if (vertex1, vertex2, vertex3) are clockwise and point is within (vertex1, vertex2, vertex3),
    // (vertex1, vertex2, point) will be clockwise
    // (vertex2, vertex3, point) will be clockwise
    // (xertex3, vertex1, point) will be clockwise


    //if (vertex1, vertex2, vertex3) are counter-clockwise and point is within (vertex1, vertex2, vertex3),
    // (vertex1, vertex2, point) will be counter-clockwise
    // (vertex2, vertex3, point) will be counter-clockwise
    // (xertex3, vertex1, point) will be counter-clockwise


    float trig1 = signed_area (vertex1, vertex2, point);
    float trig2 = signed_area (vertex2, vertex3, point);
    float trig3 = signed_area (vertex3, vertex1, point);

    if (trig1 * area >= 0 && trig2 * area >= 0 && trig3 * area >= 0) {
        return 1;
    }

    return 0;
}

I32 bbScreenCoords_withinTriangle_map (bbScreenPoints point,
    bbMapCoords vertex1, bbMapCoords vertex2, bbMapCoords vertex3)
{

}







bbMapCoords bbViewportPoints_getMapCoords_k_fixed (bbViewportPoints sc, I32 k, bbViewport* viewport ) {

    bbMapCoords mc;
    I32 vp_width = viewport->width * SCREEN_PPP;
    I32 vp_height = viewport->height * SCREEN_PPP;
    float ISOMETRIC_FACTOR = 2.0f;
    bbMapCoords viewpoint = viewport->viewpoint;
    mc.k = k;

    mc.i = (sc.x - vp_width/2.0) * POINTS_PER_PIXEL / 2 / SCREEN_PPP
           + (sc.y - vp_height/2.0) * POINTS_PER_PIXEL * ISOMETRIC_FACTOR / 2 / SCREEN_PPP
           + (mc.k - viewpoint.k) * ISOMETRIC_FACTOR / 2
           + viewpoint.i;

    mc.j = (sc.x - vp_width/2.0) * POINTS_PER_PIXEL / 2 / SCREEN_PPP
           - (sc.y - vp_height/2.0) * POINTS_PER_PIXEL * ISOMETRIC_FACTOR / 2 / SCREEN_PPP
           - (mc.k - viewpoint.k) * ISOMETRIC_FACTOR / 2
           + viewpoint.j;

    return mc;
}