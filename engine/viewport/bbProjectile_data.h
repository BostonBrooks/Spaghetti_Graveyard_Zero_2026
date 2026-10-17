#ifndef BB_PROJECTILE_DATA_H
#define BB_PROJECTILE_DATA_H
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbHandle.h"

typedef struct {
    bbMapCoords start_coords;
    bbMapCoords end_coords;
    bbTime start_time;
    bbTime end_time;
    float gravity;
    float screen_rotation;
    float screen_angle;
    bbHandle caster;
    bbListElement_Handle list;
}bbProjectile_data;

#endif //BB_PROJECTILE_DATA_H