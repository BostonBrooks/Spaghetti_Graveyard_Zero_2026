#include "engine/ECS/moveables/bbMoveables_setState.h"

#include "bbMoveables.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/ECS/ECS_instructions.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"
#include "engine/data/bbHome.h"


bbFlag bbCI_Moveable_setGoalpoint(bbCore* core,
                                  bbHandle moveable_handle,
                                  bbMapCoords goalpoint,
                                  bbInstruction_source source,
                                  bbHandle action)
{

    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbI_moveable_setState;

    instruction->data.moveable_state.handle = moveable_handle;
    instruction->data.moveable_state.type = bbMoveableType_Moving;
    instruction->data.moveable_state.goalpoint = goalpoint;



    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

bbFlag bbCI_Moveable_setMovingThrough(bbCore* core,
                                  bbHandle moveable_handle,
                                  bbMapCoords goalpoint,
                                  bbInstruction_source source,
                                  bbHandle action)
{

    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbI_moveable_setState;

    instruction->data.moveable_state.handle = moveable_handle;
    instruction->data.moveable_state.type = bbMoveableType_MovingThrough;
    instruction->data.moveable_state.goalpoint = goalpoint;



    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

bbFlag bbCI_Moveable_setGoalMovable(bbCore* core,
                             bbHandle moveable_handle,
                             bbHandle goal_moveable_handle,
                             bbInstruction_source source,
                             bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbI_moveable_setState;

    instruction->data.moveable_state.handle = moveable_handle;
    instruction->data.moveable_state.type = bbMoveableType_Following;
    instruction->data.moveable_state.goal_moveable = goal_moveable_handle.bloated.index;



    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}


bbFlag bbCI_Moveable_setGoalLunging(bbCore* core,
                             bbHandle moveable_handle,
                             bbHandle goal_moveable_handle,
                             bbInstruction_source source,
                             bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbI_moveable_setState;

    instruction->data.moveable_state.handle = moveable_handle;
    instruction->data.moveable_state.type = bbMoveableType_Lunging;
    instruction->data.moveable_state.goal_moveable = goal_moveable_handle.bloated.index;



    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}


bbFlag bbCI_Moveable_setIdle(bbCore* core,
                             bbHandle moveable_handle,
                             bbInstruction_source source,
                             bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    bbMoveable* moveable;
    bbHandle_getComponent(&home.ECS.moveables.system,(bbComponent**)&moveable,moveable_handle);

    instruction->type = bbI_moveable_setState;
    instruction->data.moveable_state.handle = moveable_handle;
    instruction->data.moveable_state.goalpoint = moveable->goalpoint;
    instruction->data.moveable_state.type = bbMoveableType_Idle;



    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

bbFlag bbI_Moveable_setState_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_moveable_unsetState;


        bbMoveables* moveables = (bbMoveables*)core->ECS->systems[bbECS_Moveables];
        bbMoveable* moveable = &moveables->moveables[instruction->data.moveable_state.handle.bloated.index];
        undo_instruction->data.moveable_state.type = moveable->type;
        undo_instruction->data.moveable_state.goalpoint = moveable->goalpoint;
        undo_instruction->data.moveable_state.goal_moveable = moveable->goal_moveable;

        undo_instruction->source = instruction->source;
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_moveable_unsetState;



        bbMoveables* moveables = (bbMoveables*)core->ECS->systems[bbECS_Moveables];
        bbMoveable* moveable = &moveables->moveables[instruction->data.moveable_state.handle.bloated.index];
        undo_instruction->data.moveable_state.type = moveable->type;
        undo_instruction->data.moveable_state.goalpoint = moveable->goalpoint;
        undo_instruction->data.moveable_state.goal_moveable = moveable->goal_moveable;

        undo_instruction->source = instruction->source;
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (instruction->source == bbInstructionSource_action)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_moveable_unsetState;


        bbMoveables* moveables = (bbMoveables*)core->ECS->systems[bbECS_Moveables];
        bbMoveable* moveable = &moveables->moveables[instruction->data.moveable_state.handle.bloated.index];
        undo_instruction->data.moveable_state.type = moveable->type;
        undo_instruction->data.moveable_state.goalpoint = moveable->goalpoint;
        undo_instruction->data.moveable_state.goal_moveable = moveable->goal_moveable;

        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    } //else source == no rewind

    bbMoveables* moveables = (bbMoveables*)core->ECS->systems[bbECS_Moveables];

    bbMoveable* moveable = &moveables->moveables[instruction->data.moveable_state.handle.bloated.index];

    moveable->type = instruction->data.moveable_state.type;
    moveable->goalpoint = instruction->data.moveable_state.goalpoint;
    moveable->goal_moveable = instruction->data.moveable_state.goal_moveable;

    bbHandle entity_handle;

    bbComponent_mapComponent(home.ECS.ECS, bbECS_Moveables,(bbComponent*)moveable, bbECS_ECS,&entity_handle,NULL);

//
    return bbSuccess;
}
bbFlag bbI_Moveable_unsetState_fn(bbCore* core, bbInstruction* instruction)
{
    bbMoveables* moveables = (bbMoveables*)core->ECS->systems[bbECS_Moveables];
    bbMoveable* moveable = &moveables->moveables[instruction->data.moveable_state.handle.bloated.index];
    moveable->type = instruction->data.moveable_state.type;
    moveable->goalpoint = instruction->data.moveable_state.goalpoint;
    moveable->goal_moveable = instruction->data.moveable_state.goal_moveable;


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

bbFlag bbCS_Moveable_setGoalpoint(bbCore* core,
                             bbHandle moveable_handle,
                             bbMapCoords goalpoint,
                             bbInstruction_source source,
                             bbHandle action)
{
    //Post undo instruction

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        bbInstruction* instruction;
        bbHandle instruction_handle;
        bbFlag flag = bbList_alloc2(&core->do_stack,(void**)&instruction, &instruction_handle);

        //set input instruction data
        instruction->type = bbI_moveable_setState;
        //bbStr_setStr(instruction->data.key, string, KEY_LENGTH);

        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction_handle;

        //set instruction data
        undo_instruction->type = bbI_moveable_unsetState;
        //bbStr_setStr(undo_instruction->data.key, test_string, KEY_LENGTH);
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = source;

        //set instruction data
        undo_instruction->type = bbI_moveable_unsetState;
        //bbStr_setStr(undo_instruction->data.key, test_string, KEY_LENGTH);
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //bbDebug("action = %d, %d\n", action.bloated.index, action.bloated.collision);
        //Set instruction data
        undo_instruction->type = bbI_moveable_unsetState;
        //bbStr_setStr(undo_instruction->data.key, test_string, KEY_LENGTH);
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_norewind)
    {

    }

    //take action
    bbMoveables* moveables = (bbMoveables*)core->ECS->systems[bbECS_Moveables];

    bbMoveable* moveable = &moveables->moveables[moveable_handle.bloated.index];

    moveable->type = bbMoveableType_Moving;
    moveable->goalpoint = goalpoint;

    bbHandle entity_handle;

    bbComponent_mapComponent(home.ECS.ECS, bbECS_Moveables,(bbComponent*)moveable, bbECS_ECS,&entity_handle,NULL);
    bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);


    return bbSuccess;
}