///A moveable stores just enough information to calculate where a monster/player
///will be at the next clock tick


#ifndef BB_MOVEABLE_H
#define BB_MOVEABLE_H
#include <pthread.h>
#include <stdatomic.h>

#include "ECS.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbVPool.h"


#define NUM_MOVEABLES 1001

typedef enum
{
    bbMoveableType_Unused,
    bbMoveableType_Idle,
    bbMoveableType_Moving,
    bbMoveableType_Follow,
    bbMoveableType_Attacking,
    bbMoveableType_Dead,
    bbMoveableType_MovingThrough


} bbMoveable_type;