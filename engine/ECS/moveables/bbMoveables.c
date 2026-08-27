
#include "bbMoveables.h"

#include "core/instructions.h"
#include "engine/core/bbAction.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"

bbFlag bbMoveable_getComponent_fn(struct bbSystem* system, bbComponent** component, bbHandle component_handle);
bbFlag bbMoveable_getHandle_fn(struct bbSystem* system, bbComponent* component, bbHandle* component_handle);

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


    I64 delta_i = (coords_a.i - coords_b.i);
    I64 delta_j = (coords_a.j - coords_b.j);
    I64 distance = bbArith64_sqrt(delta_i * delta_i + delta_j * delta_j);
    I64 gap = distance-MILLS_PER_TILE;

    I64 force = MILLS_PER_TILE/64*(M_PI_2 - atan(gap*4/MILLS_PER_TILE));


    bbMilliCoords mC;
    mC.i = delta_i * force / distance;
    mC.j = delta_j * force / distance;
    mC.k = 0;



   // static I32 counter;
   // counter++;

   // if (counter >= 10000)
   // {
   //     counter = 0;
   //     printf("%lld,%lld\n", gap, force);
   // }
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


    total.i = (double)MILLS_PER_TILE/4.f*atan((double)total.i/((double)MILLS_PER_TILE/4.f));
    total.j = (double)MILLS_PER_TILE/4.f*atan((double)total.j/((double)MILLS_PER_TILE/4.f));

    return total;
}


bbFlag bbMoveables_init(bbMoveables* moveables,bbECS* ECS)
{
    moveables->updates_per_frame = 12;
    moveables->use_coords_a = true;
    moveables->available = 0;
    bbVPool_newBloated(&moveables->snapshots, sizeof(bbMoveables_snapshot), 1000,
                       10, "bbMoveables_snapshot");

    moveables->system.getComponent = bbMoveable_getComponent_fn;
    moveables->system.getHandle = bbMoveable_getHandle_fn;

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

    bbHere()
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
                        moveable->coords_b.i += forces.i/2;
                        moveable->coords_b.j += forces.j/2;
                    }
                    else
                    {
                        //TODO use fixed point
                        double delta_i = distance_i / distance * moveable->
                            speed;
                        double delta_j = distance_j / distance * moveable->
                            speed;

                        bbMilliCoords forces = sumForces(moveables, moveable);
                        bbMilliCoords avoidables_forces =
                            bbAvoidables_sumForces(moveables, home.ECS.avoidables, moveable);


                        moveable->coords_b.i = currentLocation.i + delta_i + forces.i/2 + avoidables_forces.i;
                        moveable->coords_b.j = currentLocation.j + delta_j + forces.j/2 + avoidables_forces.j;
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

                        bbMilliCoords avoidables_forces =
                            bbAvoidables_sumForces(moveables, home.ECS.avoidables, moveable);

                        moveable->coords_b.i = currentLocation.i + delta_i + forces.i + avoidables_forces.i;
                        moveable->coords_b.j = currentLocation.j + delta_j + forces.j + avoidables_forces.j;
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
                        moveable->coords_a.i += forces.i/2;
                        moveable->coords_a.j += forces.j/2;
                    }
                    else
                    {
                        double delta_i = distance_i / distance * moveable->
                            speed;
                        double delta_j = distance_j / distance * moveable->
                            speed;

                        bbMilliCoords forces = sumForces(moveables, moveable);

                        bbMilliCoords avoidables_forces =
                            bbAvoidables_sumForces(moveables, home.ECS.avoidables, moveable);

                        moveable->coords_a.i = currentLocation.i + delta_i + forces.i/2 + avoidables_forces.i;
                        moveable->coords_a.j = currentLocation.j + delta_j + forces.j/2 + avoidables_forces.j;
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

                        bbMilliCoords avoidables_forces =
                            bbAvoidables_sumForces(moveables, home.ECS.avoidables, moveable);

                        moveable->coords_a.i = currentLocation.i + delta_i + forces.i + avoidables_forces.i;
                        moveable->coords_a.j = currentLocation.j + delta_j + forces.j + avoidables_forces.j;
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
    moveables->old_time = moveables->time;
    moveables->time = moveables->buffer_front->time;

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
        {
           moveables->moveables[i].position
                = bbMilliCoords_getMapCoords(moveables->moveables[i].coords_a);
        }else
        {
            moveables->moveables[i].position
                = bbMilliCoords_getMapCoords(moveables->moveables[i].coords_b);
        }

        moveables->moveables[i].position.k
            = bbMapCoords_getElevation(&home.ground_surface,
                                       moveables->moveables[i].position);

        moveables->buffer_back->moveables[i].ECS_entity_handle = moveables->moveables[i].component.entity_handle;


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
        target->time = moveables->time;
        target->old_time = moveables->old_time;
        moveables->buffer_fresh = false;
    }
    bbMutexUnlock(&moveables->buffer_mutex);


    return bbSuccess;
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

    moveable->component.entity_handle = ECS_entity_handle;
    moveable->moveable_handle = moveable_handle1;
    moveable->type = bbMoveableType_Moving;
    moveable->speed = 8000;

    moveable->coords_a = bbMapCoords_getMilliCoords(moveable->position);
    moveable->coords_b = bbMapCoords_getMilliCoords(moveable->position);

    *moveable_handle = moveable_handle1;
    return bbSuccess;
}


bbFlag bbMoveable_setGoalPoint(bbMoveables* moveables, bbHandle handle, bbMapCoords goalpoint)
{//TODO make core-safe?
    bbMoveable* moveable = &moveables->moveables[handle.bloated.index];


        bbDebug("index = %d\n"
             "handle = %d,%d\n"
             "entity = %d,%d\n"
            "old goalpoint = %d,%d\n"
            "new goalpoint = %d,%d\n",
            handle.bloated.index,
            moveable->moveable_handle.bloated.index,moveable->moveable_handle.bloated.collision,
            moveable->component.entity_handle.bloated.index,moveable->component.entity_handle.bloated.collision,
            moveable->goalpoint.i, moveable->goalpoint.j,
            goalpoint.i, goalpoint.j);

    moveable->goalpoint = goalpoint;
    moveable->type = bbMoveableType_Moving;



    return bbSuccess;
}

bbFlag bbMoveable_setGoalMoveable(bbMoveables* moveables, bbHandle handle, bbHandle server_handle)
{
    bbServerEntity* server_entity;

    bbVPool_lookup(home.ECS.server_entities.system.pool, (void**)&server_entity,server_handle);

    bbAssert(server_entity != NULL, "bad pool lookup\n");

    bbHandle entity_handle = server_entity->component.entity_handle;
    bbECS_entity* entity;
    bbVPool_lookup(home.ECS.ECS->system.pool, (void**)&entity,entity_handle);
    bbHandle moveable_handle = entity->components[bbECS_Moveables];

    bbMoveable* moveable = &moveables->moveables[handle.bloated.index];
    moveable->type = bbMoveableType_Follow;
    moveable->goal_moveable = moveable_handle.bloated.index;
    return bbSuccess;
}

bbFlag bbMoveable_getComponent_fn(struct bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    U32 index = component_handle.bloated.index;

    bbMoveables* moveables = (bbMoveables*)system;
    *component = (bbComponent*)&moveables->moveables[index];
    return bbSuccess;
}
bbFlag bbMoveable_getHandle_fn(struct bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    bbMoveables* moveables = (bbMoveables*)system;
    U32 offset = component - (bbComponent*)&moveables->moveables[0];
    U32 index = offset / sizeof(bbMoveable);

    component_handle->bloated.index = index;
    component_handle->bloated.collision = 193;

    return bbSuccess;
}