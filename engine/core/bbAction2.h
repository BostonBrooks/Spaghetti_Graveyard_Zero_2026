#ifndef BB_ACTION2_H
#define BB_ACTION2_H
#include "engine/data/bbConstants.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

///Different action types do different things
typedef enum
{
    bbAction2Type_printString,
    bbAction2Type_setQuote,
    bbAction2Type_unfreezeButton,
    bbAction2Type_loop
} bbAction2_type;

///Action header used to figure out what order to enact actions
typedef struct
{
    bbListElement_Handle list_element;
    U32 player;
    U32 collision;
    bbAction2_type type;
    U64 created_tick;
    U64 act_tick;
    char key[KEY_LENGTH];
} bbAction2_header;

typedef struct
{
    bbAction2_header header;
} bbAction2;

///actions are sorted by time in a total-ordering
I32 bbAction2_compare (void* A, void* B);

///Create action to be executed at a given time
bbFlag bbAction2_printString(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                 S           U64 act_tick,
                            char* key);

bbFlag bbAction2_update(void* core);
