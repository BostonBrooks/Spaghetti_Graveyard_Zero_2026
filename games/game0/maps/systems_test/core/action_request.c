#include "instructions.h"
#include "../../../../../engine/core/bbCore.h"
#include "../../../../../engine/core/bbInstruction.h"
#include "engine/core/bbAction.h"
#include "engine/core/bbInstruction_operations.h"
#include "engine/data/bbHome.h"
#include "engine/network/bbNetwork.h"
#include "engine/network/bbNetworkApp.h"

I32 bbAction_isEqual(void* A, void* B){
    bbAction* a = (bbAction*)A;
    bbAction* b = (bbAction*)B;

    if (a->header.sender == b->header.sender
        && a->header.collision == b->header.collision) return 1;
    return 0;
}


bbFlag bbAction_request(bbCore* core, bbNetwork* network, bbAction* action)
{
    static U32 collision = 0;

    action->header.collision = collision++;
    bbNetworkApp_sendAction(network, action);
    if (action->header.status == bbAction_Wait)
    { bbHere()

    } else if (action->header.status == bbAction_Speculative)
    { bbHere()

        bbDebug("sender = %u, collision = %u\n", action->header.sender, action->header.collision);


        bbAction* new_action;
        bbHandle action_handle;
        bbVPool_alloc2(core->action_pool, (void**)&new_action, &action_handle);

        //This operation nukes new_action->header.action_queue and new_action->header.all_action_list
        *new_action = *action;



        new_action->header.action_queue.list_id = 0;
        new_action->header.action_queue.prev = core->action_pool->null;
        new_action->header.action_queue.next = core->action_pool->null;
        bbList_sortL(&core->action_queue,(void*)new_action);
        new_action->header.all_actions.list_id = 0;
        new_action->header.all_actions.prev = core->action_pool->null;
        new_action->header.all_actions.next = core->action_pool->null;
        bbList_pushL(&core->all_actions,new_action);
    }

    return bbSuccess;
}


bbFlag bbAction_receive(bbCore* core, bbNetwork* network, bbAction* action)
{
bbHere()
    if (action->header.status == bbAction_Accept)
    {
        bbAction* existing_action;
        bbFlag flag = bbList_searchL(&core->all_actions,bbAction_isEqual,action,(void**)&existing_action);

        if (flag != bbSuccess)
        {
            bbAction* new_action;
            bbVPool_alloc(core->action_pool, (void**)&new_action);
            *new_action = *action;
            new_action->header.action_queue.list_id = 0;
            new_action->header.action_queue.prev = core->action_pool->null;
            new_action->header.action_queue.next = core->action_pool->null;
            bbList_sortL(&core->action_queue,(void*)new_action);
            // action->header.status = bbAction_Unknown;
            new_action->header.all_actions.list_id = 0;
            new_action->header.all_actions.prev = core->all_actions.pool->null;
            new_action->header.all_actions.next = core->all_actions.pool->null;
            bbList_pushL(&core->all_actions,new_action);
            return bbSuccess;
        }

        if (existing_action->header.status == bbAction_Speculative)
        {
            existing_action->header.status = bbAction_Accept;
            return bbSuccess;
        }

        if (existing_action->header.status == bbAction_Wait)
        {
            bbNotImplemented()
        }
    } else
    {
        bbNotHere()
    }

    return bbSuccess;

}



//TODO is an action request a rollbackable instruction? probably not!
bbFlag bbCoreInput_requestAction(bbCore* core,
                                  bbNetwork* network,
                                  bbAction* new_action,
                                  bbTime time,
                                  bbInstruction_source source,
                                  bbHandle action)
{
    allocActiveInstruction(instruction)

    bbAction* allocated;
    bbHandle allocated_handle;
    bbVPool_alloc2(core->action_pool, (void**)&allocated, &allocated_handle);

    *allocated = *new_action;

    instruction->type = bbInstruction_requestAction;
    instruction->act_time = time;
    instruction->data.three_handles.handle1 = allocated_handle;
    instruction->source = source;
    instruction->redo_instruction = action;


    pushActiveInstruction(instruction)
}



bbFlag bbInstruction_requestAction_fn(bbCore* core, bbInstruction* instruction)
{
    bbAction* action;
    bbVPool_lookup(core->action_pool, (void**)&action, instruction->data.three_handles.handle1);

    //TODO core shouldn't access home
    bbAction_request(core, &home.network, action);
    bbVPool_free(core->action_pool, (void*)action);
    ////bbVPool_free(core->instruction_pool, (void*)instruction);
    return bbSuccess;
}

bbFlag bbInstruction_unrequestAction_fn(bbCore* core, bbInstruction* instruction)
{
    bbNotImplemented()
    return bbSuccess;
}