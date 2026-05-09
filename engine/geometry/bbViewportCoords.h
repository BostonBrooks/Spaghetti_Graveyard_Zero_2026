/**
 * @file
 * @brief converts from map coordinates to sfVector2f coordinates
 *
 * How is this separated from engine/geometry ?
 **/

#ifndef BBVIEWPORTCOORDS_H
#define BBVIEWPORTCOORDS_H

#include "engine/viewport/bbViewport.h"

sfVector2f bbMapCoords_getV2f(bbMapCoords MC, bbViewport* VP);
sfVector2f bbMapCoords_getV2f_mapicon(bbMapCoords MC, bbViewport* VP);

bbViewportPoints bbMapCoords_getViewportCoords(bbMapCoords MC, bbViewport* VP);

bbMapCoords V2f_mapicon_getMapCoords(sfVector2f V2F, bbViewport* VP);
bbMapCoords bbScreenCoords_getMapCoords(bbScreenPoints SP, bbViewport* VP);

bbViewportPoints bbScreenPoints_getViewportPoints(bbViewport* viewport,
    bbScreenPoints screen_points);
#endif //BBVIEWPORTCOORDS_H