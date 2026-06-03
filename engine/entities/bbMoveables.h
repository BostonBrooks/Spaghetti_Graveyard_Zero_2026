///A moveable stores just enough information to calculate where a monster/player
///will be at the next clock tick


#ifndef BB_MOVEABLE_H
#define BB_MOVEABLE_H
#include <pthread.h>
#include <stdatomic.h>

#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbVPool.h"


#define NUM_MOVEABLES 256

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




typedef struct
{
    bbMapCoords position;
    bbMapCoords goalpoint;
} bbMoveable_snapshot;

typedef struct
{
    U64 time;
    bbMoveable_snapshot moveables[NUM_MOVEABLES];
} bbMoveables_snapshot;

typedef struct
{
    bbMoveable_type type;
    bbMapCoords position;
    I32 goal_moveable;
    bbMilliCoords coords_a;
    bbMilliCoords coords_b;
    //goalPoint could be a pointer to another avoidable, but for now it is updated once per frame
    bbMapCoords goalpoint;
    bbHandle agent2;
    I32 speed;
} bbMoveable;

typedef struct
{
    I32 updates_per_frame;
    bool use_coords_a;

    //arena allocator, could be made into a pool
    bbMoveable moveables[NUM_MOVEABLES];
    I32 available;

    bbVPool* snapshots;

    //Producer owns mutex when swapping buffers
    //Consumer owns mutex when reading from buffers
    pthread_mutex_t buffer_mutex;

    bbMoveables_snapshot buffer_a;
    bbMoveables_snapshot buffer_b;

    bbMoveables_snapshot* buffer_front;
    bbMoveables_snapshot* buffer_back;

    bool buffer_fresh;

} bbMoveables;

bbFlag bbMoveables_init(bbMoveables* moveables);
bbFlag bbMoveables_update(bbMoveables* moveables);

bbMilliCoords getForce(bbMoveables* moveables, bbMoveable* moveableA, bbMoveable* moveableB);
bbFlag bbMoveable_setGoalPoint(bbMoveables* moveables, bbHandle handle, bbMilliCoords goal_point);

bbFlag bbMoveables_copyBuffer(bbMoveables* moveables, bbMoveables_snapshot* target);

I32 bbMoveables_newSkelly(bbMoveables* moveables, bbMapCoords position, bbHandle agent);

I32 bbMoveables_newBanana(bbMoveables* moveables, bbMapCoords position, bbHandle agent, I32 moveable_index);

#endif  //BBMOVEABLE