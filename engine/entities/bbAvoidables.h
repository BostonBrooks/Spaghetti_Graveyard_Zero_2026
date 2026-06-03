#ifndef BB_AVOIDABLES_H
#define BB_AVOIDABLES_H


#include "bbMovables.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbList.h"
#include "engine/viewport/bbDrawables.h"

typedef enum
{
    bbAvoidableShape_Circle,
} bbAvoidable_shape;

typedef struct
{
    bbList_Handle listElement;
    bbMapCoords coords;
    bbAvoidable_shape shape;
    I32 radius;
} bbAvoidable;

typedef struct
{
    bbSquareCoords coords;
    bbList list;
} bbAvoidableSquare;

typedef struct
{
    bbVPool* pool;

    bbSquareCoords size;

    bbAvoidableSquare squares[];

} bbAvoidables;

bbFlag bbAvoidables_new(bbAvoidables** self, I32 squares_i, I32 squares_j);
bbFlag bbAvoidable_newCircle(bbAvoidables* avoidables, bbMapCoords MC, I32 radius);

bbMilliCoords bbAvoidables_sumForces (bbAvoidables* avoidables, bbMovable* movable);


#endif // BB_AVOIDABLES_H