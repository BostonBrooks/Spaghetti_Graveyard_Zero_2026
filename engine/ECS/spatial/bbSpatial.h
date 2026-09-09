#ifndef BB_SPATIAL_H
#define BB_SPATIAL_H


#include "engine/ECS/bbECS.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbList.h"

typedef struct
{
    bbSquareCoords coords;
    bbList list;
} bbSpatialSquare;

typedef struct
{
    bbSystem system;

    I32 squares_i;
    I32 squares_j;

    bbSpatialSquare lost;
    bbSpatialSquare squares[];


} bbAI_System;


typedef struct
{
    bbComponent component;
    bbListElement_Handle list_element;
    bbMapCoords map_coords;
    bool is_alive;
} bbSpatial_Component;

#endif //BB_SPATIAL_H