#include "instructions.h"
#include "../../../../../engine/core/bbCore.h"
#include "../../../../../engine/core/bbInstruction.h"
#include "engine/core/bbAction.h"
#include "engine/data/bbHome.h"
#include "engine/network/bbNetwork.h"
#include "engine/network/bbNetworkApp.h"


bbFlag bbAction_request(bbCore* core, bbNetwork* network, bbAction* action)
{
    if (action->header.status == bbAction_Wait)
    {

        bbNetworkApp_sendAction(network, action);
    } else
    {
        bbNotImplemented()
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
    } else
    {
        bbNotImplemented()
    }

    return bbSuccess;

}




bbFlag bbCoreInput_requestAction(bbCore* core,
                                  bbNetwork* network,
                                  bbAction* new_action,
                                  U64 time,
                                  bbInstruction_source source,
                                  bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    bbAction* allocated;
    bbHandle allocated_handle;
    bbVPool_alloc2(core->action_pool, (void**)&allocated, &allocated_handle);

    *allocated = *new_action;

    instruction->type = bbInstruction_requestAction;
    instruction->act_time = time;
    instruction->data.three_handles.handle1 = allocated_handle;
    instruction->source = source;
    instruction->redo_instruction = action;


    bbList_pushL(&core->do_stack, instruction);
}



bbFlag bbInstruction_requestAction_fn(bbCore* core, bbInstruction* instruction)
{
    bbAction* action;
    bbVPool_lookup(core->action_pool, (void**)&action, instruction->data.three_handles.handle1);

    //TODO core shouldn't access home
    bbAction_request(core, &home.network, action);
    bbVPool_free(core->action_pool, (void*)action);
    bbVPool_free(core->instruction_pool, (void*)instruction);
    return bbSuccess;
}

bbFlag bbInstruction_unrequestAction_fn(bbCore* core, bbInstruction* instruction)
{
    bbNotImplemented()
    return bbSuccess;
}