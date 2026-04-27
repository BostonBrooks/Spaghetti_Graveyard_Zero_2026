
#include "instructions.h"

#include "engine/core/bbAction.h"
#include "engine/core/bbCore.h"
#include "engine/logic/bbFlag.h"
#include "engine/pong/bbBall.h"
#include "engine/core/bbInstruction.h"
#include "engine/data/bbHome.h"
#include "engine/network/bbNetworkApp.h"
#include "engine/pong/bbPaddle.h"


extern U32 collision;


bbFlag bbVInstruction_setGoalpointOut_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_setGoalpointOut(&home.network, instruction->data.map_coords, instruction->act_time, collision++);
    return bbSuccess;
}

bbFlag bbVInstruction_setGoalpointIn_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbVInstruction_unsetGoalpoint;
    undo_instruction->data.map_coords = home.agents_app.agents.agents[0].goalpoint;
    undo_instruction->source = instruction->source;

    home.agents_app.agents.agents[0].goalpoint = instruction->data.map_coords;


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


bbFlag bbVInstruction_unsetGoalpoint_fn(bbCore* core, bbInstruction* instruction)
{
    home.agents_app.agents.agents[0].goalpoint = instruction->data.map_coords;

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

bbFlag bbVInstruction_updateMoveables_fn(bbCore* core, bbInstruction* instruction)
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

bbFlag bbVInstruction_unupdateMoveables_fn(bbCore* core, bbInstruction* instruction)
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