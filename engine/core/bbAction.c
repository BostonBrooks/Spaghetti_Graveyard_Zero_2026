#include "engine/core/bbAction.h"

#include "bbCore.h"
#include "engine/logic/bbString.h"


//create a bbAction
bbFlag bbAction_setString(void* Core,
                         U32 player,
                         U32 collision,
                         U64 created_tick,
                         U64 act_tick,
                         char* key)
{
    bbCore* core = (bbCore*)Core;
    bbAction* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->header.type = bbActionType_setString;
    action->header.player = player;
    action->header.collision = collision;
    action->header.created_tick = created_tick;
    action->header.act_tick = act_tick;
    bbStr_setStr(action->header.key, key, KEY_LENGTH);
    bbList_sortL(&core->action_queue,(void*)action);

    return bbSuccess;
}

I32 bbAction_compare (void* A, void* B)
{
    bbAction_header* a = (bbAction_header*)A;
    bbAction_header* b = (bbAction_header*)B;

    if (a->act_tick < b->act_tick) return 1;
    if (a->act_tick > b->act_tick) return 0;
    if (a->collision < b->collision) return 1;
    if (a->collision > b->collision) return 0;
    if (a->player < b->player) return 1;
    if (a->player > b->player) return 0;

    bbNotHere()

    return -1;
}