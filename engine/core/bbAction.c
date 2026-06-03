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

bbFlag bbAction_unfreezeButton(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            char* key)
{
    bbDebug("Action unfreeze button: %s\n", key);
    bbCore* core = (bbCore*)Core;
    bbAction* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->header.type = bbActionType_unfreezeButton;
    action->header.player = player;
    action->header.collision = collision;
    action->header.created_tick = created_tick;
    action->header.act_tick = act_tick;
    bbStr_setStr(action->header.key, key, KEY_LENGTH);
    bbList_sortL(&core->action_queue,(void*)action);

    return bbSuccess;
}

bbFlag bbAction_loop(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            char* key)
{
    bbCore* core = (bbCore*)Core;
    bbAction* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->header.type = bbActionType_loop;
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

bbFlag bbAction_setPaddleDirection(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            paddle_direction direction)
{


    bbCore* core = (bbCore*)Core;


    bbDebug("actual time = %lu, act tick = %lu\n", core->actual_time, act_tick);

    bbAction* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->header.type = bbActionType_setPaddleDirection;
    action->header.player = player;
    action->header.collision = collision;
    action->header.created_tick = created_tick;
    action->header.act_tick = act_tick;
    action->integer = direction;
    bbList_sortL(&core->action_queue,(void*)action);

    return bbSuccess;
}


bbFlag bbAction_setPaddleVelocity(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            I32 velocity)
{

    bbCore* core = (bbCore*)Core;


    bbDebug("actual time = %lu, act tick = %lu\n", core->actual_time, act_tick);

    bbAction* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->header.type = bbActionType_setPaddleVelocity;
    action->header.player = player;
    action->header.collision = collision;
    action->header.created_tick = created_tick;
    action->header.act_tick = act_tick;
    action->integer = velocity;
    bbList_sortL(&core->action_queue,(void*)action);

    return bbSuccess;
}


bbFlag bbAction_setViewpoint(void* Core,
                            bbMapCoords map_coords,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick)
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

    return bbSuccess;
}

bbFlag bbAction_setGoalpoint(void* Core,
                            bbMapCoords map_coords,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            U8 player)
{
    bbCore* core = (bbCore*)Core;

    bbAction* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->header.type = bbActionType_setGoalpoint;
    action->header.collision = collision;
    action->header.created_tick = created_tick;
    action->header.act_tick = act_tick;
    action->map_coords = map_coords;
    action->header.player = player;
    bbList_sortL(&core->action_queue,(void*)action);

    return bbSuccess;
}


bbFlag bbAction_spawnBanana(void* Core,
                            bbMapCoords map_coords,
                            I32 entity_index,
                            I32 movable_index,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            U8 player)
{
    bbCore* core = (bbCore*)Core;

    bbAction* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->header.type = bbActionType_spawnBanana;
    action->header.collision = collision;
    action->header.created_tick = created_tick;
    action->header.act_tick = act_tick;
    action->map_coords = map_coords;
    action->integer = entity_index;
    action->integer2 = movable_index;
    action->header.player = player;
    bbList_sortL(&core->action_queue,(void*)action);

    return bbSuccess;
}

bbFlag bbAction_spawnUnit(void* Core,
                            bbMapCoords map_coords,
                            bbMapCoords goalpoint,
                            I32 unit_type,
                            I32 entity_index,
                            I32 movable_index,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            U8 player)
{
    bbCore* core = (bbCore*)Core;


    bbAction* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->header.type = bbActionType_spawnUnit;
    action->header.collision = collision;
    action->header.created_tick = created_tick;
    action->header.act_tick = act_tick;
    action->map_coords = map_coords;
    action->goal_coords = goalpoint;
    action->integer = entity_index;
    action->integer2 = movable_index;
    action->integer3 = unit_type;
    action->header.player = player;
    bbList_sortL(&core->action_queue,(void*)action);

    return bbSuccess;
}