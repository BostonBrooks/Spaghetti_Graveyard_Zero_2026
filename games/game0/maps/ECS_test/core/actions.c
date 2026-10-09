
#include "engine/core/bbCore.h"
#include "games/game0/maps/ECS_test/core/actions.h"


bbFlag bbAction_bbHere(void* Core,
                       U32 sender,
                       U32 collision,
                       bbTime created_tick,
                       bbTime act_tick)
{
    bbCore* core = (bbCore*)Core;

    bbAction* action;
    bbList_alloc(&core->action_queue, (void**)&action);
    action->header.type = bbActionType_bbHere;
    action->header.sender = sender;
    action->header.collision = collision;
    action->header.created_tick = created_tick;
    action->header.act_tick = act_tick;
    bbList_sortL(&core->action_queue, (void*)action);


    action->header.status = bbAction_Unknown;
    action->header.all_actions.list_id = 0;
    action->header.all_actions.prev = core->all_actions.pool->null;
    action->header.all_actions.next = core->all_actions.pool->null;
    bbList_pushL(&core->all_actions,action);
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
    return bbSuccess;
}
