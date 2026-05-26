#include "engine/core/bbCore.h"
#include "engine/logic/bbFlag.h"
#include "engine/core/bbInstruction.h"

#include "bbAction.h"
#include "bbCoreInputs.h"

#ifdef DEFINE_SKELLYCHASE
#include "core/core_inputs.h"
#include "core/instructions.h"
#endif
#include "engine/data/bbHome.h"
#include "engine/logic/bbString.h"
#include "engine/network/bbNetworkApp.h"
#include "engine/userinterface/bbUI_Inbox.h"
extern char test_string[KEY_LENGTH];

U32 collision = 0;

bbFlag bbInstruction_netsendButton_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_netsendButton(&home.network, instruction->data.string);
    return bbSuccess;
}

bbFlag bbInstruction_netpauseButton_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_netpauseButton(&home.network, instruction->data.string);
    return bbSuccess;
}

bbFlag bbInstruction_netcodeButton_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_netcodeButton(&home.network, instruction->data.string, instruction->act_time);
    return bbSuccess;
}


bbFlag bbInstruction_keyUp_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_keyUp(&home.network, instruction->data.three_handles.handle1.u64, instruction->act_time, collision++);
    return bbSuccess;
}

bbFlag bbInstruction_keyDown_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_keyDown(&home.network, instruction->data.three_handles.handle1.u64, instruction->act_time, collision++);
    return bbSuccess;
}

bbFlag bbInstruction_loopAction_fn(bbCore* core, bbInstruction* instruction)
{
    bbDebug("Loop instruction at time = %lu, simulation time = %lu\n", instruction->act_time, core->simulation_time);
    bbAction_loop(core,
                  0,
                  collision++,
                  0,
                  instruction->act_time + 1,
                  instruction->data.string);
    return bbSuccess;

}

bbFlag bbInstruction_setString_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unsetString;
    bbStr_setStr(undo_instruction->data.string, test_string, KEY_LENGTH);
    undo_instruction->source = instruction->source;

    printf("    +old quote: %s, new quote %s, time = %lu\n",test_string,
        instruction->data.string, core->simulation_time);

    bbStr_setStr(test_string, instruction->data.string, KEY_LENGTH);


    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()
}


bbFlag bbInstruction_unsetString_fn(bbCore* core, bbInstruction* instruction)
{

    printf("-new quote %s, old quote: %s, time = %lu\n",
        instruction->data.string,test_string, core->simulation_time);

    bbStr_setStr(test_string,instruction->data.string, KEY_LENGTH);

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    bbAssert(0==1, "We should not get here\n");

}

bbFlag bbInstruction_unfreezeButton_fn(bbCore* core, bbInstruction* instruction)
{

bbDebug("unfreeze button %s\n", instruction->data.string);

    bbUI_Inbox_UnpressButton2(&home.UI.inbox, instruction->data.string);

    return bbSuccess;
    /* No need to undo this action?
    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    //undo_instruction->type = bbInstruction_unsetString;
    //bbStr_setStr(undo_instruction->data.string, test_string, KEY_LENGTH);
    undo_instruction->source = instruction->source;

    // printf("    +old quote: %s, new quote %s, time = %lu\n",test_string,
    //    instruction->data.string, core->simulation_time);

    //bbStr_setStr(test_string, instruction->data.string, KEY_LENGTH);


    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()
*/
}

bbFlag bbInstruction_setTime_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unsetTime;
    undo_instruction->data.unsigned_long = core->simulation_time;
    undo_instruction->source = instruction->source;

    //bbDebug ("time was %lu, now is %lu, actual %lu\n", undo_instruction->data.unsigned_long,
    //    instruction->data.unsigned_long, core->actual_time);

    core->simulation_time = instruction->data.unsigned_long ;

    //printf("+time = %lu\n", core->simulation_time);
    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()


}


bbFlag bbInstruction_unsetTime_fn(bbCore* core, bbInstruction* instruction)
{
    //bbDebug ("unset time was %lu, now is %lu, actual is %lu\n", core->simulation_time,
    //instruction->data.unsigned_long, core->actual_time);

    core->simulation_time = instruction->data.unsigned_long ;




    //printf("-time = %lu\n", core->simulation_time);
    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        //TODO place instruction->redo_instruction into core->action_queue
        bbAction* redo_action;
        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    bbNotHere()

}

bbFlag bbInstruction_checkActions_fn(bbCore* core, bbInstruction* instruction)
{

    bbAction* action;
    bbFlag flag;

    //Go back and process late arrival actions

    flag = bbList_peakL(&core->action_queue,(void**)&action);

    if (flag != bbSuccess) // list empty
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbInstruction_uncheckActions;
        //undo_instruction->data.unsigned_long = core->simulation_time;
        undo_instruction->source = instruction->source;

        if (instruction->source == bbInstructionSource_internal)
        {
            bbVPool_free(core->instruction_pool, (void*)instruction);
            undo_instruction->redo_instruction.u64 = 0;
            bbList_pushL(&core->undo_stack,(void*)undo_instruction);

        }
        if (instruction->source == bbInstructionSource_input)
        {
            bbHandle handle;
            bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
            undo_instruction->redo_instruction = handle;
            bbList_pushL(&core->undo_stack,(void*)undo_instruction);

        }
        if (instruction->source == bbInstructionSource_action)
        {
            undo_instruction->redo_instruction = instruction->redo_instruction;
            bbList_pushL(&core->undo_stack,(void*)undo_instruction);

        }

        return bbSuccess;
    }
    if (action->header.act_tick < core->simulation_time) //or < the previous time this instruction was called?
    {
        bbCore_rewindUntil(core, action->header.act_tick-1);
        bbCore_react(core);
    }

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_uncheckActions;
    undo_instruction->data.unsigned_long = core->simulation_time;
    undo_instruction->source = instruction->source;

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);

    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);

    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);

    }


    //Reverse the order of objects in queue
    flag = bbList_popL(&core->action_queue,(void**)&action);
    while (flag == bbSuccess && action->header.act_tick <= core->simulation_time)
    {
        bbList_pushL(&core->action_temp_fifo,(void*)action);
        flag = bbList_popL(&core->action_queue,(void**)&action);
    }
    //(if we go too far along in the queue, undo last instruction
    if (flag == bbSuccess) bbList_pushL(&core->action_queue,(void*)action);

    //take from one lifo and add to another
    flag = bbList_popL(&core->action_temp_fifo,(void**)&action);
    while (flag == bbSuccess)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->action_pool,action,&handle);

        if (action->header.type == bbActionType_setString)
        {
            bbCoreInput_setString(core,action->header.key,bbInstructionSource_action,handle);

        }

        if (action->header.type == bbActionType_unfreezeButton)
        {
            bbDebug("unfreeze button %s\n", action->header.key);
            bbCoreInput_unfreezeButton(core, action->header.key, bbInstructionSource_action,handle);

        }

        if (action->header.type == bbActionType_loop)
        {
            bbCoreInput_loop(core,action->header.key,action->header.act_tick,bbInstructionSource_action,handle);

        }
#ifdef DEFINE_PONG
        if (action->header.type == bbActionType_setPaddleDirection)
        {
            bbCoreInput_setPaddleDirection(core,action->integer,action->header.act_tick,bbInstructionSource_action,handle);

        }
        if (action->header.type == bbActionType_setPaddleVelocity)
        {
            bbDebug("set paddle velocity collision = %u\n", action->header.collision);
            bbCoreInput_setPaddleVelocity(core,action->header.player, action->integer,action->header.act_tick,bbInstructionSource_action,handle);

        }
#endif //DEFINE_PONG
#ifdef DEFINE_SKELLYCHASE
 //       if (action->header.type == bbActionType_setViewpoint)
//        {
//            bbCoreInput_setViewpointIn(core, action->map_coords,action->header.act_tick,bbInstructionSource_action,handle);

//        }
        if (action->header.type == bbActionType_setGoalpoint)
        {
            bbCoreInput_setGoalpointIn(core, action->map_coords,action->header.act_tick, action->header.player,bbInstructionSource_action,handle);

        }
        if (action->header.type == bbActionType_spawnBanana)
        {
            bbCoreInput_spawnBananaIn(core, action->map_coords,action->integer,
                action->integer2,action->header.act_tick,bbInstructionSource_action,handle);

        }
        if (action->header.type == bbActionType_spawnUnit)
        {
            bbCoreInput_spawnUnitIn(core, action->map_coords, action->goal_coords,action->integer3,action->integer,
                action->integer2,action->header.act_tick,bbInstructionSource_action,handle);

        }
#endif
        flag = bbList_popL(&core->action_temp_fifo,(void**)&action);
    }
    //may or may not need the following call
    //bbCore_react(core);
}

bbFlag bbInstruction_uncheckActions_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        //TODO place instruction->redo_instruction into core->action_queue
        bbAction* redo_action;
        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }




    return bbSuccess;
}

/*
bbFlag bbInstruction_setViewpointOut_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_setViewpointOut(&home.network, instruction->data.map_coords, instruction->act_time, collision++);
    return bbSuccess;
}*/

bbFlag bbInstruction_spawnBananaOut_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_spawnBananaOut(&home.network, instruction->data.map_coords, instruction->act_time, collision++);
    return bbSuccess;
}

/*
bbFlag bbInstruction_setGoalpointOut_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_setGoalpointOut(&home.network, instruction->data.map_coords, instruction->act_time, collision++);
    return bbSuccess;
}
*/
/*
bbFlag bbInstruction_setViewpointIn_fn(bbCore* core, bbInstruction* instruction)
{


        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbInstruction_unsetViewpoint;
        undo_instruction->data.map_coords = home.core.viewpoint;
        undo_instruction->source = instruction->source;

        bbUI_Inbox_SetViewpoint(&home.UI.inbox, instruction->data.map_coords);
        home.core.viewpoint = instruction->data.map_coords;

        if (instruction->source == bbInstructionSource_internal)
        {
            bbVPool_free(core->instruction_pool, (void*)instruction);
            undo_instruction->redo_instruction.u64 = 0;
            bbList_pushL(&core->undo_stack,(void*)undo_instruction);
            return bbSuccess;
        }
        if (instruction->source == bbInstructionSource_input)
        {
            bbHandle handle;
            bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
            undo_instruction->redo_instruction = handle;
            bbList_pushL(&core->undo_stack,(void*)undo_instruction);
            return bbSuccess;
        }
        if (instruction->source == bbInstructionSource_action)
        {
            undo_instruction->redo_instruction = instruction->redo_instruction;
            bbList_pushL(&core->undo_stack,(void*)undo_instruction);
            return bbSuccess;
        }
        bbNotHere()



    return bbSuccess;
}
*/
/*
bbFlag bbInstruction_setGoalpointIn_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbVInstruction_unsetGoalpoint;
    undo_instruction->data.map_coords = home.core.goalpoint;
    undo_instruction->source = instruction->source;

    home.core.goalpoint = instruction->data.map_coords;


    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()



return bbSuccess;
}
*/
#ifdef DEFINE_SKELLYCHASE
/*
bbFlag bbInstruction_approachGoalpoint_fn(bbCore* core, bbInstruction* instruction)
{


    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unapproachGoalpoint;
    undo_instruction->data.map_coords = home.core.goalpoint;
    undo_instruction->source = instruction->source;

    bbMapCoords goal, current, final;

    current = home.core.viewpoint;
    goal = home.core.goalpoint;

    //TODO: dont use doubles

    double distance_i = goal.i - current.i;
    double distance_j = goal.j - current.j;

    double distance = sqrt(distance_i * distance_i + distance_j * distance_j);
    if (distance <POINTS_PER_PIXEL*8)
   {
       final = goal;
   } else {
        double delta_i = distance_i / distance;
        double delta_j = distance_j / distance;

        bbDebug("delta_i = %f, delta_j = %f\n", delta_i, delta_j);

        final.i = current.i + delta_i * POINTS_PER_PIXEL*8;
        final.j = current.j + delta_j  * POINTS_PER_PIXEL*8;
        final.k = 0;
    }
    bbHandle noHandle = {0};

    bbCoreInput_setViewpointIn(core, final, home.core.clock2_handle.map_tick,
                                      bbInstructionSource_internal, noHandle);

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()



return bbSuccess;
}
*/
/*
bbFlag bbInstruction_unapproachGoalpoint_fn(bbCore* core, bbInstruction* instruction)
{

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }


    bbNotHere()
}
*/
/*
bbFlag bbInstruction_unsetGoalpoint_fn(bbCore* core, bbInstruction* instruction)
{
    home.core.goalpoint = instruction->data.map_coords;

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }


    bbNotHere()
}

*/

bbFlag bbInstruction_unsetViewpoint_fn(bbCore* core, bbInstruction* instruction)
{
    home.core.viewpoint = instruction->data.map_coords;

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }


    bbNotHere()
}


bbFlag bbInstruction_spawnBananaIn_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unspawnBanana;
    //undo_instruction->data.map_coords = home.agents_app.agents.agents[instruction->player].goalpoint;
    undo_instruction->source = instruction->source;
    undo_instruction->player = instruction->player;
    undo_instruction->data.unspawn.entity = home.agents_app.entities.num_entities_core;
    undo_instruction->data.unspawn.moveable = home.agents_app.movables.available;

    bbAgent* agent;



    bbAgent_newBanana(home.agents_app.agents,&agent, instruction->data.banana.position,
        instruction->data.banana.entity, instruction->data.banana.moveable);

    bbHandle agent_handle;
    bbVPool_reverseLookup(home.agents_app.agents->pool, agent, &agent_handle);

    undo_instruction->data.unspawn.agent = agent_handle;



    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()
    return bbSuccess;
}

bbFlag bbInstruction_unspawnBanana_fn(bbCore* core, bbInstruction* instruction)
{


    home.agents_app.entities.num_entities_core = instruction->data.unspawn.entity;
    home.agents_app.movables.available = instruction->data.unspawn.moveable;


    bbHandle agent_handle = instruction->data.unspawn.agent;
    bbAgent* agent;
    bbVPool_lookup(home.agents_app.agents->pool, (void**)&agent, agent_handle);
    bbAgent_deleteBanana(home.agents_app.agents, agent);


    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }


    bbNotHere()
}

#endif
#ifndef DEFINE_PONG

/*
bbFlag bbInstruction_updateMoveables_fn(bbCore* core, bbInstruction* instruction)
{


    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbVInstruction_unupdateMoveables;
    undo_instruction->source = instruction->source;

    bbMoveables_snapshot* snapshot;
    bbHandle snapshot_handle;
    bbVPool_alloc(home.agents_app.movables.snapshots, (void**)&snapshot);

    bbVPool_reverseLookup(home.agents_app.movables.snapshots, (void*)snapshot, &snapshot_handle);

    undo_instruction->snapshot = snapshot_handle;

    for (I32 i = 0; i < numMoveables; i++)
    {
        snapshot->moveables[i].position = home.agents_app.movables.moveables[i].position;
        snapshot->moveables[i].goalpoint = home.agents_app.movables.moveables[i].goalpoint;
    }



    bbMoveables_update(&home.agents_app.movables);

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()
}
*/
/*
bbFlag bbInstruction_unupdateMoveables_fn(bbCore* core, bbInstruction* instruction)
{

    bbMoveables_snapshot* snapshot;
    bbVPool_lookup(home.agents_app.movables.snapshots, (void**)&snapshot, instruction->snapshot);

    for (I32 i = 0; i < numMoveables; i++)
    {
        home.agents_app.movables.moveables[i].position = snapshot->moveables[i].position;
        home.agents_app.movables.moveables[i].goalpoint = snapshot->moveables[i].goalpoint;
    }

    bbVPool_free(home.agents_app.movables.snapshots, snapshot);

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }


    bbNotHere()
}
*/
#endif
