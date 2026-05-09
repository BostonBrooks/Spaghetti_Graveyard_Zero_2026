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