///A moveable stores just enough information to calculate where a monster/player
///will be at the next clock tick


#ifndef BB_MOVEABLE_H
#define BB_MOVEABLE_H
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbHandle.h"


#define numMoveables 256

typedef enum
{
    bbMoveableType_Unused,
    bbMoveableType_Player

} bbMoveable_type;

typedef struct
{
    bbMoveable_type type;
    bbMilliCoords coordsOriginal;
    bbMilliCoords coordsA;
    bbMilliCoords coordsB;
    //goalPoint could be a pointer to another avoidable, but for now it is updated once per frame
    bbMilliCoords goalPoint;
} bbMoveable;

typedef struct
{
    I32 updatesPerFrame;
    bool useCoordsA;
    bbMoveable moveables[numMoveables];

} bbMoveables;

bbFlag bbMoveables_init(bbMoveables* moveables);
bbFlag bbMoveables_update(bbMoveables* moveables);

bbMilliCoords getForce(bbMoveables* moveables, bbMoveable* moveableA, bbMoveable* moveableB);
bbFlag bbMoveable_setGoalPoint(bbMoveables* moveables, bbHandle handle, bbMilliCoords goal_point);


#endif  //BBMOVEABLE