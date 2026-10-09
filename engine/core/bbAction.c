#include "engine/core/bbAction.h"

#include "bbCore.h"
#include "engine/logic/bbString.h"


//create a bbAction
bbFlag bbAction_setString(void* Core,
                         U32 sender,
                         U32 collision,
                         bbTime created_tick,
                         bbTime act_tick,
                         char* key)
{
    bbCore* core = (bbCore*)Core;
    bbAction* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->header.type = bbActionType_setString;
    action->header.sender = sender;
    action->header.collision = collision;
    action->header.created_tick = created_tick;
    action->header.act_tick = act_tick;
    bbStr_setStr(action->header.key, key, KEY_LENGTH);
    bbList_sortL(&core->action_queue,(void*)action);


    action->header.status = bbAction_Unknown;
    action->header.all_actions.list_id = 0;
    action->header.all_actions.prev = core->all_actions.pool->null;
    action->header.all_actions.next = core->all_actions.pool->null;
    bbList_pushL(&core->all_actions,action);

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
    if (a->sender < b->sender) return 1;
    if (a->sender > b->sender) return 0;

    //bbNotHere()
    bbDebug("working with two identical actions!!!!!\n")
    return -1;
}


bbFlag bbAction_setViewpoint(void* Core,
                            bbMapCoords map_coords,
                            U32 collision,
                            bbTime created_tick,
                            bbTime act_tick)
{
    bbCore* core = (bbCore*)Core;

    bbAction* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->header.type = bbActionType_setViewpoint;
    action->header.collision = collision;
    action->header.created_tick = created_tick;
    action->header.act_tick = act_tick;
    action->map_coords = map_coords;
    bbList_sortL(&core->action_queue,(void*)action);

    action->header.status = bbAction_Unknown;
    action->header.all_actions.list_id = 0;
    action->header.all_actions.prev = core->all_actions.pool->null;
    action->header.all_actions.next = core->all_actions.pool->null;
    bbList_pushL(&core->all_actions,action);
    return bbSuccess;
}

