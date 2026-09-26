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
    bbActionType_setViewpoint,
    bbActionType_spawnEntity,
    bbActionType_Test,
    bbActionType_numActions
} bbAction_type;

typedef enum
{
    bbAction_Speculative,
    bbAction_Wait,
    bbAction_Accept,
    bbAction_Modify,
    bbAction_Deny,
    bbAction_numStatus,
} bbAction_status;
///Action header used to figure out what order to enact actions
typedef struct
{
    bbAction_type type;
    bbAction_status status;
    U32 sender;
    U32 collision;
    bbTime created_tick;
    bbTime act_tick;

    bbListElement_Handle list_element;
    char key[KEY_LENGTH];
} bbAction_header;

typedef struct
{
    bbAction_header header;
    I32 integer;
    I32 integer2;
    I32 integer3;
    bbMapCoords map_coords;
    bbMapCoords goal_coords;
    bbHandle handle;
} bbAction;

///actions are sorted by time in a total-ordering
I32 bbAction_compare (void* A, void* B);

///Create action to be executed at a given time
bbFlag bbAction_setString(void* Core,
                            U32 sender,
                            U32 collision,
                            bbTime created_tick,
                            bbTime act_tick,
                            char* key);



bbFlag bbAction_update(void* core);

bbFlag bbAction_loop(void* Core,
                            U32 sender,
                            U32 collision,
                            bbTime created_tick,
                            bbTime act_tick,
                            char* key);


bbFlag bbAction_setViewpoint(void* core,
                            bbMapCoords map_coords,
                            U32 collision,
                            bbTime created_tick,
                            bbTime act_tick);

bbFlag bbAction_setGoalpoint(void* Core,
                            bbMapCoords map_coords,
                            bbHandle handle,
                            U32 collision,
                            bbTime created_tick,
                            bbTime act_tick);

#endif //BB_ACTION_H