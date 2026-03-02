#ifndef BB_ACTION_H
#define BB_ACTION_H
#include "engine/data/bbData.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

///Different action types do different things
typedef enum
{
    bbActionType_setString,
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

bbFlag bbAction_update(void* core);


#endif //BB_ACTION_H