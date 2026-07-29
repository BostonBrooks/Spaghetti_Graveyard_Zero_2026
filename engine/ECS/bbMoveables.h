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




typedef struct
{
    bbHandle ECS_entity_handle;
    bbMapCoords position;
    bbMapCoords goalpoint;
    bbMoveable_type type;
} bbMoveable_snapshot;

typedef struct
{
    U64 time;
    bbMoveable_snapshot moveables[NUM_MOVEABLES];
} bbMoveables_snapshot;

typedef struct
{
    bbHandle ECS_entity_handle;
    bbHandle moveable_handle;
    bbMoveable_type type;
    //U64 last_state_change;
    bbMapCoords position;
    I32 goal_moveable;
    bbMapCoords goalpoint;
    bbMilliCoords coords_a;
    bbMilliCoords coords_b;
    I32 speed;
} bbMoveable;

typedef struct
{
    bbSystem system;
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
bbFlag bbMoveable_setGoalPoint(bbMoveables* moveables, bbHandle handle, bbMapCoords goalpoint);
bbFlag bbMoveable_setGoalMoveable(bbMoveables* moveables, bbHandle handle, bbHandle server_handle);

bbFlag bbMoveables_copyBuffer(bbMoveables* moveables, bbMoveables_snapshot* target);

bbFlag bbMoveables_newTest(bbMoveables* moveables, bbHandle* moveable_handle, bbMapCoords position, bbHandle ECS_entity_handle);

bbFlag bbCoreInput_updateMoveables(bbCore* core, bbInstruction_source source, bbHandle action);
bbFlag bbInstruction_updateMoveables_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbCoreSynchronous_spawnTestMoveable(bbCore* core,
                                           bbHandle ECS_entity_handle,
                                           bbHandle* moveable_handle,
                                           bbMapCoords position,
                                           bbInstruction_source source,
                                           bbHandle action);

bbFlag bbInstruction_spawnTestMoveable_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unspawnTestMoveable_fn(bbCore* core, bbInstruction* instruction);
#endif  //BBMoveable