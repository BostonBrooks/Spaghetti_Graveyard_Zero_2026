#include "engine/core/bbAction_check.h"

#include "entity_spawner/live_spawn.h"


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

        if (action->header.type >= bbActionType_numActions)
        {
            bbAction_fn* action_fn = core->action_functions[action->header.type- bbActionType_numActions];
            action_fn(core,action);

        }else

        if (action->header.type == bbActionType_setString)
        {
            bbCI_setString(core,action->header.key,bbInstructionSource_action,handle);

        }else
        if (action->header.type == bbActionType_spawnEntity)
        {
            //TODO virtual function / callback
            //bbCoreInput_spawnTestEntity(core,&home.ECS.ECS, action->map_coords, action->handle, bbInstructionSource_action, handle);
            bbSpawnFunctionArgs args;
            args.position = action->map_coords;
            args.goalpoint = action->map_coords;
            bbCI_live_spawnEntity(core, args, "SKELLY", bbInstructionSource_internal, no_handle);

        }

        flag = bbList_popL(&core->action_temp_fifo,(void**)&action);
    }
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
        bbAction* redo_action;
        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }

    return bbSuccess;
}