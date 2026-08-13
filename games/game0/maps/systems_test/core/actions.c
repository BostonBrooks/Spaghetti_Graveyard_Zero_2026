
#include "engine/core/bbCore.h"
#include "games/game0/maps/systems_test/core/actions.h"


#include "games/game0/maps/systems_test/core/action_set_goalpoint.h"

bbFlag bbAction_bbHere(void* Core,
                       U32 player,
                       U32 collision,
                       U64 created_tick,
                       U64 act_tick)
{
    bbCore* core = (bbCore*)Core;

    bbAction* action;
    bbList_alloc(&core->action_queue, (void**)&action);
    action->header.type = bbActionType_bbHere;
    action->header.player = player;
    action->header.collision = collision;
    action->header.created_tick = created_tick;
    action->header.act_tick = act_tick;
    bbList_sortL(&core->action_queue, (void*)action);

    return bbSuccess;
}

bbFlag bbAction_bbHere_fn(bbCore* core, bbAction* action)
{
    bbHere()
    return bbSuccess;
}

bbFlag bbCore_initActions(bbCore* core)
{
    core->action_functions = calloc(bbActionType_numVActions - bbActionType_numActions, sizeof(bbAction_fn*));

    core->action_functions[bbActionType_bbHere- bbActionType_numActions] = bbAction_bbHere_fn;
    core->action_functions[bbActionType_setGoalpoint- bbActionType_numActions] = bbAction_setGoalpoint_fn;
    return bbSuccess;
}