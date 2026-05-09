#include "engine/geometry/bbViewportCoords.h"

#include "engine/data/bbConstants.h"
#include "engine/userinterface/bbWidgets.h"


sfVector2f bbMapCoords_getV2f(bbMapCoords MC, bbViewport* VP){
    sfVector2f V2F;
    V2F.x = VP->width/2.0
            + (MC.i - VP->viewpoint.i) / (1.0 * POINTS_PER_PIXEL)
            + (MC.j - VP->viewpoint.j) / (1.0 * POINTS_PER_PIXEL);

    V2F.y = VP->height/2.0
            + (MC.i - VP->viewpoint.i) / (2.0 * POINTS_PER_PIXEL)
            - (MC.j - VP->viewpoint.j) / (2.0 * POINTS_PER_PIXEL)
            - (MC.k - VP->viewpoint.k) / (1.0 * POINTS_PER_PIXEL);

    return V2F;
}

sfVector2f bbMapCoords_getV2f_mapicon(bbMapCoords MC, bbViewport* VP){
    sfVector2f V2F;
    V2F.x = VP->width/2.0
          + (MC.i - VP->viewpoint.i) / (1.0 * POINTS_PER_PIXEL * MAPICON_SCALE)
          + (MC.j - VP->viewpoint.j) / (1.0 * POINTS_PER_PIXEL * MAPICON_SCALE);

    V2F.y = VP->height/2.0
          + (MC.i - VP->viewpoint.i) / (2.0 * POINTS_PER_PIXEL * MAPICON_SCALE)
          - (MC.j - VP->viewpoint.j) / (2.0 * POINTS_PER_PIXEL * MAPICON_SCALE);

    return V2F;
}

bbMapCoords V2f_mapicon_getMapCoords(sfVector2f V2F, bbViewport* VP){
	bbMapCoords MC;
	MC.i = ((V2F.x / 2 + V2F.y - VP->height/2 - VP->width / 4) * POINTS_PER_PIXEL * MAPICON_SCALE) + VP->viewpoint.i;
	MC.j = ((V2F.x / 2 - V2F.y + VP->height/2 - VP->width / 4) * POINTS_PER_PIXEL * MAPICON_SCALE) + VP->viewpoint.j;
	MC.k = 0;
	return MC;
}

//Assume POINTS_PER_PIXEL == SCREEN_PPP
bbScreenPoints bbMapCoords_getSP(bbMapCoords MC, bbViewport* VP){
    bbAssert(POINTS_PER_PIXEL == SCREEN_PPP, "Fix Coordinate System!\n");
    bbScreenPoints SP;
    SP.x = VP->width*SCREEN_PPP/2.0
            + (MC.i - VP->viewpoint.i)/2
            + (MC.j - VP->viewpoint.j)/2;

    SP.y = VP->height*SCREEN_PPP/2.0
            + (MC.i - VP->viewpoint.i)/2
            - (MC.j - VP->viewpoint.j)/2
            - (MC.k - VP->viewpoint.k);

    return SP;
}

// SP is relative to top left of viewport see SP_to_MC_algebra.txt
bbMapCoords bbScreenCoords_getMapCoords(bbScreenPoints SP, bbViewport* VP){
    bbAssert(POINTS_PER_PIXEL == SCREEN_PPP, "Fix Coordinate System!\n");
    bbMapCoords MC;

    MC.i = (SP.x - VP->width*SCREEN_PPP/2.0)/2
           + SP.y - VP->height*SCREEN_PPP/2.0 - VP->viewpoint.k +
           VP->viewpoint.i;
    MC.j = (SP.x - VP->width*SCREEN_PPP/2.0)/2
           - SP.y + VP->height*SCREEN_PPP/2.0 + VP->viewpoint.k +
           VP->viewpoint.j;
    MC.k = 0;

    return MC;
}


bbViewportPoints bbScreenPoints_getViewportPoints(bbViewport* viewport,
    bbScreenPoints screen_points)
{
    bbWidget* viewport_widget = viewport->widget;
    bbScreenPoints viewport_position;
    viewport_position.x = viewport_widget->rect.left;
    viewport_position.y = viewport_widget->rect.top;

    bbViewportPoints VP;
    VP.x = screen_points.x - viewport_position.x;
    VP.y = screen_points.y - viewport_position.y;
    return VP;

}