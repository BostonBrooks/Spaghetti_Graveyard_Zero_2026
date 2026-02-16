#ifndef BB_ACTION_H
#define BB_ACTION_H
#include "engine/data/bbConstants.h"
#include "engine/logic/bbHandle.h"

typedef enum
{
    bbActionType_printString,
    bbActionType_setQuote,
    bbActionType_unfreezeButton
} bbAction_type;

typedef  struct
{
    bbListElement_Handle list_element;
    U32 player;
    U32 collision;
    bbAction_type type;
    U64 created_tick;
    U64 act_tick;
    char key[KEY_LENGTH];

} bbAction;


///return A < B
I32 bbAction_compare (void* A, void* B);

bbFlag bbAction_printString(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            char* key);

bbFlag bbAction_setQuote(void* Core,
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


bbFlag bbActions_react(void* core, U64 tick_time);
#endif // BB_ACTION_H