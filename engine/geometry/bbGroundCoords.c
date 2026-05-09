#include "engine/geometry/bbGroundCoords.h"

#include <iso646.h>

#include "bbViewportCoords.h"
#include "engine/data/bbConstants.h"
#include "engine/data/bbHome.h"
#include "engine/viewport/bbViewport.h"


//bbMapCoords bbScreenCoords_getMapCoords(bbScreenPoints SP);
//I32 bbTileCoords_getElevation(bbGroundSurface* surface, bbTileCoords coords);
//I32 bbMapCoords_getElevation(bbGroundSurface* ground_surface, bbMapCoords coords);
//static I32 bbArith_div (I32 x, I32 y);
//bbSquareCoords bbMapCoords_getSquareCoords(bbMapCoords MC);

#ifdef DEBUG_GROUNGSURFACE
bbDrawable *orange_dot,*yellow_dot,*green_dot,*cyan_dot,*cobalt_dot,*magenta_dot,*red_dot;
#endif

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

float signed_area (bbViewportCoords p1, bbViewportCoords p2, bbViewportCoords p3) {
    //positive is clockwise

    float x1, y1, x2, y2;

    x1 = p2.x - p1.x;
    y1 = p2.y - p1.y;

    x2 = p3.x - p2.x;
    y2 = p3.y - p2.y;

    float area = (x1 * y2 - x2 * y1) / 2;

    return area;
}

I32 bbViewportCoords_withinTriangle_screen (bbViewportCoords point,
    bbViewportCoords vertex1, bbViewportCoords vertex2, bbViewportCoords vertex3){


    float area = signed_area (vertex1, vertex2, vertex3);

    if (area == 0) {
        bbHere()
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

I32 bbViewportCoords_withinTriangle_map (bbViewportCoords point,
    bbMapCoords vertex1, bbMapCoords vertex2, bbMapCoords vertex3)
{
    bbViewportCoords VP1, VP2, VP3;

    VP1 = bbMapCoords_getViewportCoords(vertex1, &home.viewport_app.viewport);
    VP2 = bbMapCoords_getViewportCoords(vertex2, &home.viewport_app.viewport);
    VP3 = bbMapCoords_getViewportCoords(vertex3, &home.viewport_app.viewport);

    return bbViewportCoords_withinTriangle_screen (point, VP1, VP2, VP3);

}

I32 bbViewportCoords_withinTile (bbTileCoords tile, bbViewportCoords p) {
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




    if (bbViewportCoords_withinTriangle_map (p, left_vertex, top_vertex, right_vertex) != 0){

        return 1; //Top

    }

    if (bbViewportCoords_withinTriangle_map (p, left_vertex, bottom_vertex, right_vertex ) != 0){

        return 2; //Bottom
    }

    return 0;
}

bbMapCoords bbViewportCoords_interpolateMapCoords (bbViewportCoords p, bbMapCoords vertex1, bbMapCoords vertex2, bbMapCoords vertex3){

    bbViewportCoords v1, v2, v3;

    v1 = bbMapCoords_getViewportCoords(vertex1, &home.viewport_app.viewport);
    v2 = bbMapCoords_getViewportCoords(vertex2, &home.viewport_app.viewport);
    v3 = bbMapCoords_getViewportCoords(vertex3, &home.viewport_app.viewport);

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

    bbAssert(det4 != 0, "Matrix singular!\n");

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



bbMapCoords bbViewportCoords_getMapCoords_k_fixed (bbViewportCoords sc, I32 k, bbViewport* viewport ) {

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

int bbMapCoords_withinTile_LR (bbMapCoords point) {

    //TODO how does % operator work with negative values of point.i
    int residual_i = point.i % (POINTS_PER_TILE);
    int residual_j = point.j % (POINTS_PER_TILE);

    if (residual_i + residual_j < POINTS_PER_TILE){
        return 1; // Left
    } else {
        return 0; // Right
    }
}

bbMapCoords bbViewportCoords_getMapCoords (bbViewportCoords point){


#ifdef DEBUG_GROUNGSURFACE
    if (orange_dot == NULL)
    {

        bbMapCoords no_coords;
        no_coords.i = 1;
        no_coords.j = 1;
        no_coords.k = 1;
        bbDrawable_newPoint(&orange_dot, home.viewport_app.drawables,
        &home.UI.graphics, no_coords);
        orange_dot->frames[0].handle.u64 = 617;
        bbDrawable_newPoint(&yellow_dot, home.viewport_app.drawables,
        &home.UI.graphics, no_coords);
        yellow_dot->frames[0].handle.u64 = 618;
        bbDrawable_newPoint(&green_dot, home.viewport_app.drawables,
        &home.UI.graphics, no_coords);
        green_dot->frames[0].handle.u64 = 619;
        bbDrawable_newPoint(&cyan_dot, home.viewport_app.drawables,
        &home.UI.graphics, no_coords);
        cyan_dot->frames[0].handle.u64 = 620;
        bbDrawable_newPoint(&cobalt_dot, home.viewport_app.drawables,
        &home.UI.graphics, no_coords);
        cobalt_dot->frames[0].handle.u64 = 621;
        bbDrawable_newPoint(&magenta_dot, home.viewport_app.drawables,
        &home.UI.graphics, no_coords);
        magenta_dot->frames[0].handle.u64 = 622;
        bbDrawable_newPoint(&red_dot, home.viewport_app.drawables,
                          &home.UI.graphics, no_coords);
        red_dot->frames[0].handle.u64 = 616;
    }

#endif
    bbMapCoords point_max_k = bbViewportCoords_getMapCoords_k_fixed (point, 2000, &home.viewport_app.viewport );
    // the highest point on the map that could correspond to the point on the screen

    //return point_max_k;

    bbMapCoords point_0_k = bbViewportCoords_getMapCoords_k_fixed (point, 00, &home.viewport_app.viewport );
    // the lowest point on the map that could correspond to the point on the screen


#ifdef DEBUG_GROUNGSURFACE
    bbDrawable_setLocation(orange_dot, home.viewport_app.drawables,point_0_k);
#endif
    point_0_k = bbViewportCoords_getMapCoords_k_fixed (point, -100, &home.viewport_app.viewport );

    //return point_0_k;

    int point_left_or_right = bbMapCoords_withinTile_LR (point_max_k);
    // is the point to the left or right of the middle of the first tile examined

    int point_top_or_bottom;
    // is the point in the northern or southern half of the tile being examined,
    // or neither.

    bbTileCoords tile_point_0_k = bbMapCoords_getTileCoords (point_0_k);
    bbTileCoords tile =  bbMapCoords_getTileCoords (point_max_k);

/*
    bbMapCoords test_vertex;
    test_vertex.i = tile_point_0_k.i * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    test_vertex.j = tile_point_0_k.j * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    test_vertex.k = 0;//(&home.ground_surface, left_vertex);
    bbDrawable_setLocation(test_drawable_6, home.viewport_app.drawables,test_vertex);


    test_vertex.i = tile_point_0_k.i * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    test_vertex.j = tile_point_0_k.j * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    test_vertex.k = 0;//(&home.ground_surface, left_vertex);
    bbDrawable_setLocation(test_drawable_5, home.viewport_app.drawables,test_vertex);
*/
    if (point_left_or_right == 1){
    //point is on the left side of the first tile we look at;

        while (tile.j <= tile_point_0_k.j){

            point_top_or_bottom = bbViewportCoords_withinTile(tile, point);


            if (point_top_or_bottom != 0) break;

            tile.i--;

            point_top_or_bottom = bbViewportCoords_withinTile(tile, point);

            if (point_top_or_bottom != 0) break;

            tile.j++;

        }

    } else {
    //point is on the right side of the first tile we look at;

        while (tile.j <= tile_point_0_k.j){



            point_top_or_bottom = bbViewportCoords_withinTile(tile, point);


            if (point_top_or_bottom != 0) break;

            tile.j++;

            point_top_or_bottom = bbViewportCoords_withinTile(tile, point);

            if (point_top_or_bottom != 0) break;

            tile.i--;

        }

    }

    bbMapCoords left_vertex;
    left_vertex.i = tile.i * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    left_vertex.j = tile.j * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    left_vertex.k = bbMapCoords_getElevation(&home.ground_surface, left_vertex);

#ifdef DEBUG_GROUNGSURFACE
    bbDrawable_setLocation(yellow_dot, home.viewport_app.drawables,left_vertex);
#endif

    //assert (point_top_or_bottom != 0);
    if (point_top_or_bottom == 0){


        bbMapCoords foo;
        foo.i = -1;
        foo.j = -1;
        foo.k=  -1;


        //bbDrawable_setLocation(test_drawable_1, home.viewport_app.drawables,foo);
        //bbDrawable_setLocation(test_drawable_2, home.viewport_app.drawables,foo);
        //bbDrawable_setLocation(test_drawable_3, home.viewport_app.drawables,foo);
        //bbDrawable_setLocation(test_drawable_4, home.viewport_app.drawables,foo);

        return foo;

    }

    bbMapCoords middle_vertex, right_vertex;



    right_vertex.i = (tile.i + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    right_vertex.j = (tile.j + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    right_vertex.k =  bbMapCoords_getElevation(&home.ground_surface, right_vertex);

#ifdef DEBUG_GROUNGSURFACE
    bbDrawable_setLocation(green_dot, home.viewport_app.drawables,right_vertex);
#endif
    if (point_top_or_bottom == 1){ //Top

        middle_vertex.i = tile.i * POINTS_PER_PIXEL * PIXELS_PER_TILE;
        middle_vertex.j = (tile.j + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
        middle_vertex.k = bbMapCoords_getElevation(&home.ground_surface, middle_vertex);

#ifdef DEBUG_GROUNGSURFACE
        bbDrawable_setLocation(cyan_dot, home.viewport_app.drawables,middle_vertex);
#endif
    } else { //bottom

        middle_vertex.i = (tile.i + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
        middle_vertex.j = tile.j * POINTS_PER_PIXEL * PIXELS_PER_TILE;
        middle_vertex.k = bbMapCoords_getElevation(&home.ground_surface, middle_vertex);


#ifdef DEBUG_GROUNGSURFACE
        bbDrawable_setLocation(cyan_dot, home.viewport_app.drawables,middle_vertex);
#endif
    }



    bbMapCoords foo;
    foo = bbViewportCoords_interpolateMapCoords (point, left_vertex, middle_vertex, right_vertex);

#ifdef DEBUG_GROUNGSURFACE
    bbDrawable_setLocation(cobalt_dot, home.viewport_app.drawables,foo);
#endif
    return foo;
}