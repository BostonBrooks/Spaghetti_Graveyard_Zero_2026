
#ifndef BBECS_PROJECT_DATA_H
#define BBECS_PROJECT_DATA_H

#include "engine/geometry/bbCoordinates.h"
typedef struct {

    bbMapCoords start_coords;
    bbMapCoords current_coords;
    bbMapCoords end_coords;
    bbTime start_time;
    bbTime end_time;
    U64 gravity;
} bbProjectileMovement_data;



#endif //BBECS_PROJECT_DATA_H