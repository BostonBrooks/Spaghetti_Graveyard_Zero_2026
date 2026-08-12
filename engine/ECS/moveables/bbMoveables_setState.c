#include "engine/ECS/moveables/bbMoveables_setState.h"

#include "bbMoveables.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/ECS/ECS_instructions.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"


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
    instruction->data.moveable_state.type = bbMoveableType_Follow;
    instruction->data.moveable_state.goal_moveable = goal_moveable_handle.bloated.index;



    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
}


bbFlag bbCI_Moveable_setIdle(bbCore* core,
                             bbHandle moveable_handle,
                             bbHandle goal_moveable_handle,
                             bbInstruction_source source,
                             bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbI_moveable_setState;

    instruction->data.moveable_state.type = bbMoveableType_Idle;



    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
}

bbFlag bbI_Moveable_setState_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_moveable_unsetState;

//TODO Lookup old moveable state

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


        //TODO Lookup old moveable state


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

        //TODO Lookup old moveable state

        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    } //else source == no rewind

    //TODO set new moveable state

    bbStr_setStr(test_string, instruction->data.key, KEY_LENGTH);

    return bbSuccess;
}
bbFlag bbI_Moveable_unsetState_fn(bbCore* core, bbInstruction* instruction)
{
    bbNotHere()
}

