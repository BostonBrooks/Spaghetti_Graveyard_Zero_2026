#ifndef BB_GROUND_COORDS_H
#define BB_GROUND_COORDS_H


#include "engine/geometry/bbCoordinates.h"
#include "engine/viewport/bbViewport.h"

bbMapCoords bbViewportPoints_getMapCoords(bbViewportPoints VP);
bbMapCoords bbViewportPoints_getMapCoords_k_fixed (bbViewportPoints sc, I32 k, bbViewport* viewport );

#endif  // BB_GROUND_COORDS_H
