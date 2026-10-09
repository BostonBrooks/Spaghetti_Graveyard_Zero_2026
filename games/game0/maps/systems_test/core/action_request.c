#include "instructions.h"
#include "../../../../../engine/core/bbCore.h"
#include "../../../../../engine/core/bbInstruction.h"
#include "engine/core/bbAction.h"
#include "engine/core/bbInstruction_operations.h"
#include "engine/data/bbHome.h"
#include "engine/network/bbNetwork.h"
#include "engine/network/bbNetworkApp.h"


bbFlag bbAction_request(bbCore* core, bbNetwork* network, bbAction* action)
{
    if (action->header.status == bbAction_Wait)
    { bbHere()

        bbNetworkApp_sendAction(network, action);
    } else if (action->header.status == bbAction_Speculative)
    { bbHere()

        bbAction* new_action;
        bbFlag flag = bbList_alloc(&core->action_queue,(void**)&new_action);


        //This operation nukes new_action->header.action_queue and new_action->header.all_action_list
        *new_action = *action;



        new_action->header.action_queue.list_id = 0;
        new_action->header.action_queue.prev = core->action_queue.pool->null;
        new_action->header.action_queue.next = core->action_queue.pool->null;
        bbList_sortL(&core->action_queue,(void*)new_action);
        new_action->header.all_action_list.list_id = 0;
        new_action->header.all_action_list.prev = core->all_action_list.pool->null;
        new_action->header.all_action_list.next = core->all_action_list.pool->null;
        bbList_pushL(&core->all_action_list,new_action);
    }

    return bbSuccess;
}


bbFlag bbAction_receive(bbCore* core, bbNetwork* network, bbAction* action)
{

    if (action->header.status == bbAction_Accept)
    {
        bbAction* new_action;
        bbFlag flag = bbList_alloc(&core->action_queue,(void**)&new_action);

        *new_action = *action;


        bbList_sortL(&core->action_queue,(void*)new_action);



       // action->header.status = bbAction_Unknown;
        action->header.all_action_list.list_id = 0;
        action->header.all_action_list.prev = core->all_action_list.pool->null;
        action->header.all_action_list.next = core->all_action_list.pool->null;
        bbList_pushL(&core->all_action_list,action);
    } else
    {
        bbNotImplemented()
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