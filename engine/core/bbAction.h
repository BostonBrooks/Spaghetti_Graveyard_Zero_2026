#ifndef BB_ACTION_H
#define BB_ACTION_H
#include "engine/data/bbConstants.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

///Different action types do different things
typedef enum
{
    bbActionType_setString,
    bbActionType_unfreezeButton,
    bbActionType_loop,
    bbActionType_setPaddleDirection,
    bbActionType_setPaddleVelocity,
    bbActionType_setViewpoint,
    bbActionType_setGoalpoint,
    bbActionType_spawnBanana
} bbAction_type;

///Action header used to figure out what order to enact actions
typedef struct
{
    bbListElement_Handle list_element;
    U32 player;
    U32 collision;
    bbAction_type type;
    U64 created_tick;
    U64 act_tick;
    char key[KEY_LENGTH];
} bbAction_header;

typedef struct
{
    bbAction_header header;
    I32 integer;
    I32 integer2;
    bbMapCoords map_coords;
} bbAction;

///actions are sorted by time in a total-ordering
I32 bbAction_compare (void* A, void* B);

///Create action to be executed at a given time
bbFlag bbAction_setString(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            char* key);


bbFlag bbAction_unfreezeButton(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            char* key);

bbFlag bbAction_update(void* core);

bbFlag bbAction_loop(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            char* key);

typedef enum
{
    left_up,
    left_stop,
    left_down,
    right_up,
    right_stop,
    right_down,
} paddle_direction;

bbFlag bbAction_setPaddleDirection(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            paddle_direction direction);

bbFlag bbAction_setPaddleVelocity(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            I32 velocity);

bbFlag bbAction_setViewpoint(void* core,
                            bbMapCoords map_coords,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick);

bbFlag bbAction_setGoalpoint(void* Core,
                            bbMapCoords map_coords,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            U8 player);

bbFlag bbAction_spawnBanana(void* Core,
                            bbMapCoords position,
                            I32 entity_index,
                            I32 moveable_index,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            U8 player);

#endif //BB_ACTION_H