#ifndef BB_GROUND_COORDS_H
#define BB_GROUND_COORDS_H


#include "engine/geometry/bbCoordinates.h"
#include "engine/viewport/bbViewport.h"

bbMapCoords bbViewportCoords_getMapCoords(bbViewportCoords VP);
bbMapCoords bbViewportCoords_getMapCoords_k_fixed (bbViewportCoords sc, I32 k, bbViewport* viewport );
bbMapCoords bbViewportCoords_interpolateMapCoords (bbViewportCoords p,
    bbMapCoords vertex1, bbMapCoords vertex2, bbMapCoords vertex3);
#endif  // BB_GROUND_COORDS_H
