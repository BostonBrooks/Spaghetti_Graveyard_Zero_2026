#include "engine/geometry/bbGroundCoords.h"

#include "bbViewportCoords.h"
#include "engine/data/bbConstants.h"
#include "engine/data/bbHome.h"
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

I32 bbViewportPoints_withinTriangle_map (bbViewportPoints point,
    bbMapCoords vertex1, bbMapCoords vertex2, bbMapCoords vertex3)
{
    bbViewportPoints VP1, VP2, VP3;

    VP1 = bbMapCoords_getViewportCoords(vertex1, &home.viewport_app.viewport);
    VP2 = bbMapCoords_getViewportCoords(vertex2, &home.viewport_app.viewport);
    VP3 = bbMapCoords_getViewportCoords(vertex3, &home.viewport_app.viewport);

    return bbViewportPoints_withinTriangle_screen (point, VP1, VP2, VP3);

}

I32 bbViewportPoints_withinTile (bbTileCoords tile, bbViewportPoints p) {
    // is the point in the northern or southern half of the tile being examined,
    // or neither.

    bbMapCoords left_vertex, top_vertex, right_vertex, bottom_vertex;

    bbTileCoords left_tile, top_tile, right_tile, bottom_tile;

    left_tile = top_tile = right_tile = bottom_tile = tile;

    top_tile.j++;
    right_tile.i++;
    right_tile.j++;
    bottom_tile.i++;

    left_vertex.i = tile.i * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    left_vertex.j = tile.j * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    left_vertex.k = bbTileCoords_getElevation(&home.ground_surface,left_tile);

    top_vertex.i = tile.i * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    top_vertex.j = (tile.j + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    top_vertex.k = bbTileCoords_getElevation(&home.ground_surface,top_tile);

    right_vertex.i = (tile.i + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    right_vertex.j = (tile.j + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    right_vertex.k = bbTileCoords_getElevation(&home.ground_surface,right_tile);

    bottom_vertex.i = (tile.i + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    bottom_vertex.j = tile.j * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    bottom_vertex.k = bbTileCoords_getElevation(&home.ground_surface,bottom_tile);



    if (bbViewportPoints_withinTriangle_map (p, left_vertex, top_vertex, right_vertex) != 0){
        return 1; //Top
    }

    if (bbViewportPoints_withinTriangle_map (p, left_vertex, bottom_vertex, right_vertex ) != 0){
        return 2; //Bottom
    }


    return 0;
}

bbMapCoords bbScreenCoords_interpolateMapCoords (bbScreenCoords p, bbMapCoords vertex1, bbMapCoords vertex2, bbMapCoords vertex3){

    bbScreenCoords v1, v2, v3;

    v1 = bbMapCoords_getScreenCoords_centre(vertex1);
    v2 = bbMapCoords_getScreenCoords_centre(vertex2);
    v3 = bbMapCoords_getScreenCoords_centre(vertex3);

    bbMapCoords point;

    float det1, det2, det3, det4;

    det1 = vertex1.i * v2.y + vertex2.i * v3.y + vertex3.i * v1.y
         - vertex1.i * v3.y - vertex2.i * v1.y - vertex3.i * v2.y;

    det2 = v1.x * vertex2.i + v2.x * vertex3.i + v3.x * vertex1.i
         - v1.x * vertex3.i - v2.x * vertex1.i - v3.x * vertex2.i;

    det3 = v1.x * v2.y * vertex3.i + v2.x * v3.y * vertex1.i + v3.x * v1.y * vertex2.i
         - v1.x * v3.y * vertex2.i - v2.x * v1.y * vertex3.i - v3.x * v2.y * vertex1.i   ;


    det4 = v1.x * v2.y + v2.x * v3.y + v3.x * v1.y
         - v1.x * v3.y - v2.x * v1.y - v3.x * v2.y;

    assert(det4 != 0);

    point.i = (det1 * p.x + det2 * p.y + det3) / det4;



    det1 = vertex1.j * v2.y + vertex2.j * v3.y + vertex3.j * v1.y
         - vertex1.j * v3.y - vertex2.j * v1.y - vertex3.j * v2.y;

    det2 = v1.x * vertex2.j + v2.x * vertex3.j + v3.x * vertex1.j
         - v1.x * vertex3.j - v2.x * vertex1.j - v3.x * vertex2.j;

    det3 = v1.x * v2.y * vertex3.j + v2.x * v3.y * vertex1.j + v3.x * v1.y * vertex2.j
         - v1.x * v3.y * vertex2.j - v2.x * v1.y * vertex3.j - v3.x * v2.y * vertex1.j;


    //det4 = v1.x * v2.y + v2.x * v3.y + v3.x * v1.y
    //     - v1.x * v3.y - v2.x * v1.y - v3.x * v2.y;

    //assert(det4 != 0);

    point.j = (det1 * p.x + det2 * p.y + det3) / det4;



    det1 = vertex1.k * v2.y + vertex2.k * v3.y + vertex3.k * v1.y
         - vertex1.k * v3.y - vertex2.k * v1.y - vertex3.k * v2.y;

    det2 = v1.x * vertex2.k + v2.x * vertex3.k + v3.x * vertex1.k
         - v1.x * vertex3.k - v2.x * vertex1.k - v3.x * vertex2.k;

    det3 = v1.x * v2.y * vertex3.k + v2.x * v3.y * vertex1.k + v3.x * v1.y * vertex2.k
         - v1.x * v3.y * vertex2.k - v2.x * v1.y * vertex3.k - v3.x * v2.y * vertex1.k;


    //det4 = v1.x * v2.y + v2.x * v3.y + v3.x * v1.y
    //     - v1.x * v3.y - v2.x * v1.y - v3.x * v2.y;

    //assert(det4 != 0);

    point.k = (det1 * p.x + det2 * p.y + det3) / det4;

    return point;
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