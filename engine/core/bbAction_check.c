#include "engine/core/bbAction_check.h"

#include "bbInstruction_operations.h"
#include "engine/ECS/moveables/bbMoveables.h"
#include "engine/viewport/bbDrawables.h"
#include "games/game0/maps/systems_test/entity_spawner/live_spawn.h"

//#define NO_ROLLBACK

bbFlag bbInstruction_checkActions_fn(bbCore* core, bbInstruction* instruction)
{

bbHere()

    bbAction* action;
    bbFlag flag;

    //Go back and process late arrival actions

    flag = bbList_peakL(&core->action_queue,(void**)&action);

    if (flag != bbSuccess) // list empty
    {
        if (instruction->source == bbInstructionSource_internal)
        {
            allocUndoInstruction(undo_instruction)
            undo_instruction->type = bbInstruction_uncheckActions;
            undo_instruction->source = instruction->source;
            bbVPool_free(core->instruction_pool, (void*)instruction);
            undo_instruction->redo_instruction.u64 = 0;
            pushUndoInstruction(undo_instruction)
        }
        else if (instruction->source == bbInstructionSource_input)
        {
            allocUndoInstruction(undo_instruction)
            undo_instruction->type = bbInstruction_uncheckActions;
            undo_instruction->source = instruction->source;
            allocRedoInstruction(redo_instruction)
             *redo_instruction = *instruction;
            undo_instruction->redo_instruction = redo_instruction_handle;
            pushRedoInstruction(redo_instruction)
            pushUndoInstruction(undo_instruction)
        }
        else if (instruction->source == bbInstructionSource_action)
        {
            allocUndoInstruction(undo_instruction)
            undo_instruction->type = bbInstruction_uncheckActions;
            undo_instruction->source = instruction->source;
            undo_instruction->redo_instruction = instruction->redo_instruction;
            pushUndoInstruction(undo_instruction)

        } //else source == no rewind

        return bbSuccess;
    }
#ifndef NO_ROLLBACK
    if (action->header.act_tick < core->simulation_time) //or < the previous time this instruction was called?
    {bbHere()
        bbCore_rewindUntil(core, action->header.act_tick-1);
        bbHere()
        bbCore_react(core);
        bbHere()
    }
#endif //NO_ROLLBACK

bbHere()

    //Reverse the order of objects in queue
    flag = bbList_popL(&core->action_queue,(void**)&action);
    while (flag == bbSuccess && action->header.act_tick <= core->simulation_time)
    {
        bbList_pushL(&core->action_temp_fifo,(void*)action);
        flag = bbList_popL(&core->action_queue,(void**)&action);
    }

    bbHere()
    //(if we go too far along in the queue, undo last instruction
    if (flag == bbSuccess) bbList_pushL(&core->action_queue,(void*)action);

    bbHere()
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

        }
#ifdef DEFINE_TEST_SYSTEM //TODO move from game engine to game code
        else
        if (action->header.type == bbActionType_spawnEntity)
        {
            bbSpawnFunctionArgs args;
            args.position = action->map_coords;
            args.goalpoint = action->map_coords;
            args.speed = 8000;
            args.radius = 65536;
            args.mass = 512;
            args.state = bbMoveableType_Moving;
            args.type = 0;
            args.handle = no_handle;
            args.goal_handle = no_handle;
            bbCI_live_spawnEntity(core, args, "SKELLY_LIVE", bbInstructionSource_action, handle);

        }
#endif
        flag = bbList_popL(&core->action_temp_fifo,(void**)&action);
    }

    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbInstruction_uncheckActions;
        undo_instruction->source = instruction->source;
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbInstruction_uncheckActions;
        undo_instruction->source = instruction->source;
        allocRedoInstruction(redo_instruction)
         *redo_instruction = *instruction;
        undo_instruction->redo_instruction = redo_instruction_handle;
        pushRedoInstruction(redo_instruction)
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_action)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbInstruction_uncheckActions;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)

    } //else source == no rewind
bbHere()
    return bbSuccess;
}

bbFlag bbInstruction_uncheckActions_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        bbHere()
                return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        popRedoInstruction(redo_instruction,instruction)
        allocActiveInstruction(new_instruction)
        *new_instruction = redo_instruction;
        pushActiveInstruction(new_instruction)
bbHere()
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
    }

    return bbSuccess;
}