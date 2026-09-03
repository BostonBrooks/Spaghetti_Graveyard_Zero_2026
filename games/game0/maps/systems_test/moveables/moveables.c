#include "moveables.h"
#include "core/instructions.h"
#include "engine/core/bbAction.h"
#include "engine/data/bbHome.h"

bbFlag bbCoreSynchronous_spawnTestMoveable(bbCore* core,
                                           bbHandle ECS_entity_handle,
                                           bbHandle* moveable_handle,
                                           bbMapCoords position,
                                           I32 speed,
                                           I32 radius,
                                           I32 mass,
                                           bbInstruction_source source,
                                           bbHandle action)
{ bbHere()
    I32 index = home.ECS.moveables.available++;

    bbHandle moveable_handle1;
    moveable_handle1.bloated.index = index;
    moveable_handle1.bloated.collision = 193;


    bbMoveable* moveable = &home.ECS.moveables.moveables[index];

    moveable->position = position;
    position.i += POINTS_PER_SQUARE;
    moveable->goalpoint = position;


    moveable->component.entity_handle = ECS_entity_handle;
    moveable->moveable_handle = moveable_handle1;

    // bbDebug("entity handle: %d,%d\n"
    //         "moveable handle: %d,%d\n",
    //         moveable->component.entity_handle.bloated.index,
    //         moveable->component.entity_handle.bloated.collision,
    //         moveable->moveable_handle.bloated.index,
    //         moveable->moveable_handle.bloated.collision);


    moveable->type = bbMoveableType_Moving;
    moveable->speed = speed;
    moveable->radius = radius;
    moveable->mass = mass;

    moveable->coords_a = bbMapCoords_getMilliCoords(moveable->position);
    moveable->coords_b = bbMapCoords_getMilliCoords(moveable->position);



    bbCS_entity_setComponent(core,
                             home.ECS.ECS,
                             ECS_entity_handle,
                             moveable_handle1,
                             bbECS_Moveables,
                             bbInstructionSource_internal,
                             no_handle);

    *moveable_handle = moveable_handle1;

    if (source == bbInstructionSource_norewind) return  bbSuccess;

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unspawnTestMoveable;
    undo_instruction->source = source;
    undo_instruction->data.three_handles.handle1 = moveable_handle1;
    if (source == bbInstructionSource_internal)
    {
        //bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
        return bbSuccess;
    }
    //if (source == bbInstructionSource_input)?
    //if (source == bbInstructionSource_action)?


    bbNotHere()
}


bbFlag bbInstruction_spawnTestMoveable_fn(bbCore* core, bbInstruction* instruction)
{
    bbNotHere()
}

bbFlag bbInstruction_unspawnTestMoveable_fn(bbCore* core, bbInstruction* instruction)
{
    U32 index = instruction->data.three_handles.handle1.bloated.index;
    bbMoveable* moveable = &home.ECS.moveables.moveables[index];
    moveable->type = bbMoveableType_Dead;

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

}


bbFlag bbCoreInput_updateMoveables(bbCore* core,
                                  bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbInstruction_updateMoveables;
    instruction->source = source;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

bbFlag bbInstruction_updateMoveables_fn(bbCore* core,
                                        bbInstruction* instruction)
{
    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unupdateMoveables;
    undo_instruction->source = instruction->source;

    bbMoveables_snapshot* snapshot;
    bbHandle snapshot_handle;
    bbFlag flag = bbVPool_alloc2(home.ECS.moveables.snapshots, (void**)&snapshot,&snapshot_handle);


    undo_instruction->snapshot = snapshot_handle;

    for (I32 i = 0; i < NUM_MOVEABLES; i++)
    {
        snapshot->moveables[i].ECS_entity_handle = home.ECS.moveables.moveables[i].component.entity_handle;
        snapshot->moveables[i].position = home.ECS.moveables.moveables[i].
            position;
         snapshot->moveables[i].goalpoint = home.ECS.moveables.moveables[i].
            goalpoint;
    }
    snapshot->time = home.ECS.moveables.time;
    snapshot->old_time = home.ECS.moveables.old_time;

    bbMoveables_update(&home.ECS.moveables);

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()
}



bbFlag bbInstruction_unupdateMoveables_fn(bbCore* core,

bbInstruction* instruction)
{
    bbMoveables_snapshot* snapshot;
    bbFlag flag  = bbVPool_lookup(home.ECS.moveables.snapshots, (void**)&snapshot,
                   instruction->snapshot);

    for (I32 i = 0; i < NUM_MOVEABLES; i++)
    {
        home.ECS.moveables.moveables[i].position = snapshot->moveables[i].
        position;
        home.ECS.moveables.moveables[i].goalpoint = snapshot->moveables[i].
            goalpoint;
    }
    home.ECS.moveables.time = snapshot->time;
    home.ECS.moveables.old_time = snapshot->old_time;

    bbVPool_free(home.ECS.moveables.snapshots, snapshot);

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction,
                       instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action,
                       instruction->redo_instruction);
        bbList_sortL(&core->action_queue, (void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }


    bbNotHere()
}
