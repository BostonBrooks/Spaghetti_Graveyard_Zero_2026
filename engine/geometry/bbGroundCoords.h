#ifndef BB_GROUND_COORDS_H
#define BB_GROUND_COORDS_H


#include "engine/geometry/bbCoordinates.h"
#include "engine/viewport/bbViewport.h"

bbMapCoords bbScreenCoords_getMapCoords(bbScreenPoints SP);
bbMapCoords bbScreenCoords_getMapCoords_k_fixed (bbScreenPoints sc, I32 k, bbViewport* viewport );

#endif  // BB_GROUND_COORDS_H
