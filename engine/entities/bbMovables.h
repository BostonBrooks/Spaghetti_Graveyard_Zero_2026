///A movable stores just enough information to calculate where a monster/player
///will be at the next clock tick


#ifndef BB_Movable_H
#define BB_Movable_H
#include <pthread.h>
#include <stdatomic.h>

#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbVPool.h"


#define NUM_MOVABLES 256

typedef enum
{
    bbMovableType_Unused,
    bbMovableType_Idle,
    bbMovableType_Moving,
    bbMovableType_Follow,
    bbMovableType_Attacking,
    bbMovableType_Dead,
    bbMovableType_MovingThrough


} bbMovable_type;




typedef struct
{
    bbMapCoords position;
    bbMapCoords goalpoint;
} bbMovable_snapshot;

typedef struct
{
    U64 time;
    bbMovable_snapshot movables[NUM_MOVABLES];
} bbMovables_snapshot;

typedef struct
{
    bbMovable_type type;
    U64 last_state_change;
    bbMapCoords position;
    I32 goal_movable;
    bbMilliCoords coords_a;
    bbMilliCoords coords_b;
    //goalPoint could be a pointer to another avoidable, but for now it is updated once per frame
    bbMapCoords goalpoint;
    bbHandle agent2;
    I32 speed;
} bbMovable;

typedef struct
{
    I32 updates_per_frame;
    bool use_coords_a;

    //arena allocator, could be made into a pool
    bbMovable movables[NUM_MOVABLES];
    I32 available;

    bbVPool* snapshots;

    //Producer owns mutex when swapping buffers
    //Consumer owns mutex when reading from buffers
    pthread_mutex_t buffer_mutex;

    bbMovables_snapshot buffer_a;
    bbMovables_snapshot buffer_b;

    bbMovables_snapshot* buffer_front;
    bbMovables_snapshot* buffer_back;

    bool buffer_fresh;

} bbMovables;

bbFlag bbMovables_init(bbMovables* movables);
bbFlag bbMovables_update(bbMovables* movables);

bbMilliCoords getForce(bbMovables* movables, bbMovable* movableA, bbMovable* movableB);
bbFlag bbMovable_setGoalPoint(bbMovables* movables, bbHandle handle, bbMilliCoords goal_point);

bbFlag bbMovables_copyBuffer(bbMovables* movables, bbMovables_snapshot* target);

I32 bbMovables_newSkelly(bbMovables* movables, bbMapCoords position, bbHandle agent);

I32 bbMovables_newBanana(bbMovables* movables, bbMapCoords position, bbHandle agent, I32 movable_index);

#endif  //BBMovable