
#include "engine/ECS/bbMoveables.h"

#include "engine/core/bbAction.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"

bbMilliCoords getForce(bbMoveables* moveables, bbMoveable* moveableA,
                       bbMoveable* moveableB)
{


    bbMilliCoords coords_a, coords_b, output;

    if (moveables->use_coords_a)
    {
        coords_a = moveableA->coords_a;
        coords_b = moveableB->coords_a;
    }
    else
    {
        coords_a = moveableA->coords_b;
        coords_b = moveableB->coords_b;
    }


    double delta_i = (coords_a.i - coords_b.i);
    double delta_j = (coords_a.j - coords_b.j);
    double distance = sqrt(delta_i * delta_i + delta_j * delta_j);
    double distanceReduced = (distance - 0.5l * MILLS_PER_TILE) / 10000.l;
    double distanceReduced2 = distance / 100000.l;

    bbMilliCoords mC;
    mC.i = ((delta_i) / (distanceReduced2 * distanceReduced * distanceReduced));
    mC.j = ((delta_j) / (distanceReduced2 * distanceReduced * distanceReduced));
    mC.k = 0;

    return mC;
}

bbMilliCoords sumForces(bbMoveables* moveables, bbMoveable* moveableA)
{
    bbMoveable* moveableB;
    bbMilliCoords single, total;
    total.i = 0;
    total.j = 0;
    total.k = 0;

    for (I32 i = 0; i < NUM_MOVEABLES; i++)
    {
        moveableB = &moveables->moveables[i];
        if (moveableB->type == bbMoveableType_Unused) continue;
        if (moveableB->type == bbMoveableType_MovingThrough) continue;
        if (moveableB->type == bbMoveableType_Dead) continue;
        if (moveableA == moveableB) continue;
        single = getForce(moveables, moveableA, moveableB);
        total.i += single.i;
        total.j += single.j;
    }
    return total;
}


bbFlag bbMoveables_init(bbMoveables* moveables,bbECS* ECS)
{
    moveables->updates_per_frame = 12;
    moveables->use_coords_a = true;
    moveables->available = 0;
    bbVPool_newBloated(&moveables->snapshots, sizeof(bbMoveables_snapshot), 100,
                       100, "bbMoveables_snapshot");

    moveables->buffer_back = &moveables->buffer_a;
    moveables->buffer_front = &moveables->buffer_b;
    moveables->buffer_fresh = true;

    pthread_mutex_init(&moveables->buffer_mutex,NULL);

    for (I32 i = 0; i < NUM_MOVEABLES; i++)
    {
        bbMoveable* moveable = &moveables->moveables[i];

        moveable->type = bbMoveableType_Unused;
    }


    ECS->systems[bbECS_Moveables] = (bbSystem* )moveables;
    return bbSuccess;
}

bbFlag bbMoveables_updateOnce(bbMoveables* moveables)
{
    //for now, just move 2048 mills toward goal point
    if (moveables->use_coords_a)
    {
        for (I32 i = 0; i < NUM_MOVEABLES; i++)
        {
            bbMoveable* moveable = &moveables->moveables[i];
            switch (moveable->type)
            {
            case bbMoveableType_Unused:
            case bbMoveableType_Idle:
            case bbMoveableType_Attacking:
            case bbMoveableType_Dead:

                moveable->coords_b.i = moveable->coords_a.i;
                moveable->coords_b.j = moveable->coords_a.j;
                break;

            case bbMoveableType_Moving:
                {
                    bbMilliCoords currentLocation = moveable->coords_a;
                    bbMilliCoords goalPoint
                        = bbMapCoords_getMilliCoords(moveable->goalpoint);


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(
                        distance_i * distance_i + distance_j * distance_j);

                    if (distance < moveable->speed)
                    {
                        moveable->coords_b = goalPoint;
                        bbMilliCoords forces = sumForces(moveables, moveable);
                        moveable->coords_b.i += forces.i;
                        moveable->coords_b.j += forces.j;
                    }
                    else
                    {
                        //TODO use fixed point
                        double delta_i = distance_i / distance * moveable->
                            speed;
                        double delta_j = distance_j / distance * moveable->
                            speed;

                        bbMilliCoords forces = sumForces(moveables, moveable);
                        //bbMilliCoords avoidables_forces =
                        //    bbAvoidables_sumForces(
                        //        home.agents_app.avoidables, moveable);

                        moveable->coords_b.i = currentLocation.i + delta_i + forces.i;// + avoidables_forces.i;
                        moveable->coords_b.j = currentLocation.j + delta_j + forces.j;// + avoidables_forces.j;
                    }
                    // moveable->position = moveable->coords_b;
                }
                break;
            case bbMoveableType_MovingThrough:
                {
                    bbMilliCoords currentLocation = moveable->coords_a;
                    bbMilliCoords goalPoint
                        = bbMapCoords_getMilliCoords(moveable->goalpoint);


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(
                        distance_i * distance_i + distance_j * distance_j);

                    if (distance < moveable->speed)
                    {
                        moveable->coords_b = goalPoint;
                    }
                    else
                    {
                        double delta_i = distance_i / distance * moveable->
                            speed;
                        double delta_j = distance_j / distance * moveable->
                            speed;


                        moveable->coords_b.i = currentLocation.i + delta_i;
                        moveable->coords_b.j = currentLocation.j + delta_j;
                    }
                    // moveable->position = moveable->coords_b;
                }
                break;
            case bbMoveableType_Follow:
                {
                    moveable->goalpoint = moveables->moveables[moveable->
                        goal_moveable].position;
                    bbMilliCoords currentLocation = moveable->coords_a;
                    bbMilliCoords goalPoint
                        = bbMapCoords_getMilliCoords(moveable->goalpoint);


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(
                        distance_i * distance_i + distance_j * distance_j);

                    if (distance < moveable->speed)
                    {
                        moveable->coords_b = goalPoint;
                    }
                    else
                    {
                        //TODO use fixed point
                        double delta_i = distance_i / distance * moveable->
                            speed;
                        double delta_j = distance_j / distance * moveable->
                            speed;

                        bbMilliCoords forces = sumForces(moveables, moveable);
                        //bbMilliCoords avoidables_forces =
                        //    bbAvoidables_sumForces(
                        //        home.agents_app.avoidables, moveable);

                        moveable->coords_b.i = currentLocation.i + delta_i + forces.i;// + avoidables_forces.i;
                        moveable->coords_b.j = currentLocation.j + delta_j + forces.j;// + avoidables_forces.j;
                    }
                }
                break;
            }
        }

        moveables->use_coords_a = false;
        return bbSuccess;
    }
    else
    {
        for (I32 i = 0; i < NUM_MOVEABLES; i++)
        {
            bbMoveable* moveable = &moveables->moveables[i];
            switch (moveable->type)
            {
            case bbMoveableType_Unused:
            case bbMoveableType_Idle:
            case bbMoveableType_Attacking:
            case bbMoveableType_Dead:

                moveable->coords_a.i = moveable->coords_b.i;
                moveable->coords_a.j = moveable->coords_b.j;
                break;
            case bbMoveableType_Moving:
                {
                    bbMilliCoords currentLocation = moveable->coords_b;
                    bbMilliCoords goalPoint
                        = bbMapCoords_getMilliCoords(moveable->goalpoint);


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(
                        distance_i * distance_i + distance_j * distance_j);

                    if (distance < moveable->speed)
                    {
                        moveable->coords_a = goalPoint;
                        bbMilliCoords forces = sumForces(moveables, moveable);
                        moveable->coords_a.i += forces.i;
                        moveable->coords_a.j += forces.j;
                    }
                    else
                    {
                        double delta_i = distance_i / distance * moveable->
                            speed;
                        double delta_j = distance_j / distance * moveable->
                            speed;

                        bbMilliCoords forces = sumForces(moveables, moveable);
                        //bbMilliCoords avoidables_forces =
                        //    bbAvoidables_sumForces(
                        //        home.agents_app.avoidables, moveable);

                        moveable->coords_a.i = currentLocation.i + delta_i + forces.i;// + avoidables_forces.i;
                        moveable->coords_a.j = currentLocation.j + delta_j + forces.j;// + avoidables_forces.j;
                    }
                }
                break;

            case bbMoveableType_MovingThrough:
                {
                    bbMilliCoords currentLocation = moveable->coords_b;
                    bbMilliCoords goalPoint
                        = bbMapCoords_getMilliCoords(moveable->goalpoint);


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(
                        distance_i * distance_i + distance_j * distance_j);

                    if (distance < moveable->speed)
                    {
                        moveable->coords_a = goalPoint;
                    }
                    else
                    {
                        double delta_i = distance_i / distance * moveable->
                            speed;
                        double delta_j = distance_j / distance * moveable->
                            speed;


                        moveable->coords_a.i = currentLocation.i + delta_i;
                        moveable->coords_a.j = currentLocation.j + delta_j;
                    }
                }
                break;
            case bbMoveableType_Follow:
                {
                    moveable->goalpoint = moveables->moveables[moveable->
                        goal_moveable].position;
                    bbMilliCoords currentLocation = moveable->coords_b;
                    bbMilliCoords goalPoint
                        = bbMapCoords_getMilliCoords(moveable->goalpoint);


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(
                        distance_i * distance_i + distance_j * distance_j);

                    if (distance < moveable->speed)
                    {
                        moveable->coords_a = goalPoint;
                    }
                    else
                    {
                        double delta_i = distance_i / distance * moveable->
                            speed;
                        double delta_j = distance_j / distance * moveable->
                            speed;

                        bbMilliCoords forces = sumForces(moveables, moveable);
                        //bbMilliCoords avoidables_forces =
                        //    bbAvoidables_sumForces(
                        //        home.agents_app.avoidables, moveable);

                        moveable->coords_a.i = currentLocation.i + delta_i + forces.i;// + avoidables_forces.i;
                        moveable->coords_a.j = currentLocation.j + delta_j + forces.j;// + avoidables_forces.j;
                    }
                }
                break;
            }
        }

        moveables->use_coords_a = true;
        return bbSuccess;
    }
}


bbFlag bbMoveables_update(bbMoveables* moveables)
{
    for (I32 i = 0; i < NUM_MOVEABLES; i++)
    {
        //moveables->moveables[i].goalpoint = moveables->moveables[0].position;

        moveables->moveables[i].coords_a = bbMapCoords_getMilliCoords(
            moveables->moveables[i].position);
        moveables->moveables[i].coords_b = bbMapCoords_getMilliCoords(
            moveables->moveables[i].position);
    }

    for (I32 i = 0; i < moveables->updates_per_frame; i++)
    {
        bbMoveables_updateOnce(moveables);
    }

    for (I32 i = 0; i < NUM_MOVEABLES; i++)
    {
        if (moveables->use_coords_a)
            moveables->moveables[i].position
                = bbMilliCoords_getMapCoords(moveables->moveables[i].coords_a);
        else
            moveables->moveables[i].position
                = bbMilliCoords_getMapCoords(moveables->moveables[i].coords_b);

        moveables->moveables[i].position.k
            = bbMapCoords_getElevation(&home.ground_surface,
                                       moveables->moveables[i].position);

        moveables->buffer_back->moveables[i].ECS_entity_handle = moveables->moveables[i].ECS_entity_handle;
        moveables->buffer_back->moveables[i].goalpoint = moveables->moveables[i]
            .goalpoint;
        moveables->buffer_back->moveables[i].position = moveables->moveables[i].
            position;
        moveables->buffer_back->moveables[i].type = moveables->moveables[i].type;
        moveables->buffer_back->time = home.core.clock2_handle.map_tick;
    }
    if (home.core.core.simulation_time == home.core.core.actual_time)
    {
        bbMutexLock(&moveables->buffer_mutex);

        bbMoveables_snapshot* temp = moveables->buffer_back;
        moveables->buffer_back = moveables->buffer_front;
        moveables->buffer_front = temp;

        moveables->buffer_fresh = true;

        bbMutexUnlock(&moveables->buffer_mutex);
    }

    return bbSuccess;
}


bbFlag bbMoveables_copyBuffer(bbMoveables* moveables,
                              bbMoveables_snapshot* target)
{
    bbMutexLock(&moveables->buffer_mutex);

    if (moveables->buffer_fresh == true)
    {
        for (I32 i = 0; i < NUM_MOVEABLES; i++)
        {
            target->moveables[i].ECS_entity_handle
                = moveables->buffer_front->moveables[i].ECS_entity_handle;
            target->moveables[i].goalpoint = moveables->buffer_front->moveables[i].goalpoint;
            target->moveables[i].position = moveables->buffer_front->moveables[i].position;
            target->moveables[i].type = moveables->moveables[i].type;

        }
        target->time = moveables->buffer_front->time;
        moveables->buffer_fresh = false;
    }
    bbMutexUnlock(&moveables->buffer_mutex);


    return bbSuccess;
}
bbFlag bbCoreSynchronous_spawnTestMoveable(bbCore* core,
                                           bbHandle ECS_entity_handle,
                                           bbHandle* moveable_handle,
                                           bbMapCoords position,
                                           bbInstruction_source source,
                                           bbHandle action)
{ bbHere()
    I32 index = home.ECS.moveables.available++;

    bbHandle moveable_handle1;
    moveable_handle1.bloated.index = index;
    moveable_handle1.bloated.collision = 193;


    bbMoveable* moveable = &home.ECS.moveables.moveables[index];
    moveable->goalpoint = position;
    moveable->position = position;

    moveable->ECS_entity_handle = ECS_entity_handle;
    moveable->moveable_handle = moveable_handle1;
    moveable->type = bbMoveableType_Moving;
    moveable->speed = 8000;

    moveable->coords_a = bbMapCoords_getMilliCoords(moveable->position);
    moveable->coords_b = bbMapCoords_getMilliCoords(moveable->position);

    bbCS_entity_setComponent(core,
                             &home.ECS.ECS,
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

bbFlag bbMoveables_newTest(bbMoveables* moveables, bbHandle* moveable_handle, bbMapCoords position, bbHandle ECS_entity_handle)
{
    I32 index = moveables->available++;

    bbHandle moveable_handle1;
    moveable_handle1.bloated.index = index;
    moveable_handle1.bloated.collision = 193;


    bbMoveable* moveable = &moveables->moveables[index];
    moveable->goalpoint = position;
    moveable->position = position;

    moveable->ECS_entity_handle = ECS_entity_handle;
    moveable->moveable_handle = moveable_handle1;
    moveable->type = bbMoveableType_Moving;
    moveable->speed = 8000;

    moveable->coords_a = bbMapCoords_getMilliCoords(moveable->position);
    moveable->coords_b = bbMapCoords_getMilliCoords(moveable->position);

    *moveable_handle = moveable_handle1;
    return bbSuccess;
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
        snapshot->moveables[i].ECS_entity_handle = home.ECS.moveables.moveables[i].ECS_entity_handle;
        snapshot->moveables[i].position = home.ECS.moveables.moveables[i].
            position;
        snapshot->moveables[i].goalpoint = home.ECS.moveables.moveables[i].
            goalpoint;
    }


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


bbFlag bbMoveable_setGoalPoint(bbMoveables* moveables, bbHandle handle, bbMapCoords goalpoint)
{
    bbMoveable* moveable = &moveables->moveables[handle.bloated.index];

    moveable->goalpoint = goalpoint;

    return bbSuccess;
}

bbFlag bbMoveable_setGoalMoveable(bbMoveables* moveables, bbHandle handle, bbHandle server_handle)
{
    bbServerEntity* server_entity;

    bbVPool_lookup(home.ECS.server_entities.system.pool, (void**)&server_entity,server_handle);
    bbHandle entity_handle = server_entity->bbECS_entity_handle;
    bbECS_entity* entity;
    bbVPool_lookup(home.ECS.ECS.system.pool, (void**)&entity,entity_handle);
    bbHandle moveable_handle = entity->components[bbECS_Moveables];

    bbMoveable* moveable = &moveables->moveables[handle.bloated.index];
    moveable->type = bbMoveableType_Follow;
    moveable->goal_moveable = moveable_handle.bloated.index;
    return bbSuccess;
}