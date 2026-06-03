#include "engine/entities/bbMovables.h"

#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"

bbMilliCoords getForce(bbMovables* movables, bbMovable* movableA,
                       bbMovable* movableB)
{
    //SpriteUnits[subject].Forces.i += (10000*idist)/dist/(dist - footprint)/(dist - footprint);
    //from Spaghetti_Graveyard_Demos/OldNoTerrainDemo/06_Units.h

    bbMilliCoords coords_a, coords_b, output;

    if (movables->use_coords_a)
    {
        coords_a = movableA->coords_a;
        coords_b = movableB->coords_a;
    }
    else
    {
        coords_a = movableA->coords_b;
        coords_b = movableB->coords_b;
    }


    double delta_i = (coords_a.i - coords_b.i);
    double delta_j = (coords_a.j - coords_b.j);
    double distance = sqrt(delta_i * delta_i + delta_j * delta_j);
    double distanceReduced = (distance - 0.2l * MILLS_PER_TILE) / 10000.l;
    double distanceReduced2 = distance / 100000.l;

    bbMilliCoords mC;
    mC.i = ((delta_i) / (distanceReduced2 * distanceReduced * distanceReduced));
    mC.j = ((delta_j) / (distanceReduced2 * distanceReduced * distanceReduced));
    mC.k = 0;

    return mC;
}

bbMilliCoords sumForces(bbMovables* movables, bbMovable* movableA)
{
    bbMovable* movableB;
    bbMilliCoords single, total;
    total.i = 0;
    total.j = 0;
    total.k = 0;

    for (I32 i = 0; i < NUM_MOVABLES; i++)
    {
        movableB = &movables->movables[i];
        if (movableB->type == bbMovableType_Unused) continue;
        if (movableB->type == bbMovableType_MovingThrough) continue;
        if (movableB->type == bbMovableType_Dead) continue;
        if (movableA == movableB) continue;
        single = getForce(movables, movableA, movableB);
        total.i += single.i;
        total.j += single.j;
    }
    return total;
}


bbFlag bbMovables_init(bbMovables* movables)
{
    movables->updates_per_frame = 12;
    movables->use_coords_a = true;
    movables->available = 0;
    bbVPool_newBloated(&movables->snapshots, sizeof(bbMovables_snapshot), 100,
                       100, "bbMovables_snapshot");

    movables->buffer_back = &movables->buffer_a;
    movables->buffer_front = &movables->buffer_b;
    movables->buffer_fresh = true;

    pthread_mutex_init(&movables->buffer_mutex,NULL);

    for (I32 i = 0; i < NUM_MOVABLES; i++)
    {
        bbMovable* movable = &movables->movables[i];

        movable->type = bbMovableType_Unused;
    }
    return bbSuccess;
}

bbFlag bbMovables_updateOnce(bbMovables* movables)
{
    //for now, just move 2048 mills toward goal point
    if (movables->use_coords_a)
    {
        for (I32 i = 0; i < NUM_MOVABLES; i++)
        {
            bbMovable* movable = &movables->movables[i];
            switch (movable->type)
            {
            case bbMovableType_Unused:
            case bbMovableType_Idle:
            case bbMovableType_Attacking:
            case bbMovableType_Dead:

                movable->coords_b.i = movable->coords_a.i;
                movable->coords_b.j = movable->coords_a.j;
                break;

            case bbMovableType_Moving:
                {
                    bbMilliCoords currentLocation = movable->coords_a;
                    bbMilliCoords goalPoint
                        = bbMapCoords_getMilliCoords(movable->goalpoint);


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(
                        distance_i * distance_i + distance_j * distance_j);

                    if (distance < movable->speed)
                    {
                        movable->coords_b = goalPoint;
                    }
                    else
                    {
                        //TODO use fixed point
                        double delta_i = distance_i / distance * movable->
                            speed;
                        double delta_j = distance_j / distance * movable->
                            speed;

                        bbMilliCoords forces = sumForces(movables, movable);
                        bbMilliCoords avoidables_forces =
                            bbAvoidables_sumForces(
                                home.agents_app.avoidables, movable);

                        movable->coords_b.i = currentLocation.i + delta_i +
                            forces.i + avoidables_forces.i;
                        movable->coords_b.j = currentLocation.j + delta_j +
                            forces.j + avoidables_forces.j;
                    }
                    // movable->position = movable->coords_b;
                }
                break;
            case bbMovableType_MovingThrough:
                {
                    bbMilliCoords currentLocation = movable->coords_a;
                    bbMilliCoords goalPoint
                        = bbMapCoords_getMilliCoords(movable->goalpoint);


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(
                        distance_i * distance_i + distance_j * distance_j);

                    if (distance < movable->speed)
                    {
                        movable->coords_b = goalPoint;
                    }
                    else
                    {
                        double delta_i = distance_i / distance * movable->
                            speed;
                        double delta_j = distance_j / distance * movable->
                            speed;


                        movable->coords_b.i = currentLocation.i + delta_i;
                        movable->coords_b.j = currentLocation.j + delta_j;
                    }
                    // movable->position = movable->coords_b;
                }
                break;
            case bbMovableType_Follow:
                {
                    movable->goalpoint = movables->movables[movable->
                        goal_movable].position;
                    bbMilliCoords currentLocation = movable->coords_a;
                    bbMilliCoords goalPoint
                        = bbMapCoords_getMilliCoords(movable->goalpoint);


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(
                        distance_i * distance_i + distance_j * distance_j);

                    if (distance < movable->speed)
                    {
                        movable->coords_b = goalPoint;
                    }
                    else
                    {
                        //TODO use fixed point
                        double delta_i = distance_i / distance * movable->
                            speed;
                        double delta_j = distance_j / distance * movable->
                            speed;

                        bbMilliCoords forces = sumForces(movables, movable);
                        bbMilliCoords avoidables_forces =
                            bbAvoidables_sumForces(
                                home.agents_app.avoidables, movable);

                        movable->coords_b.i = currentLocation.i + delta_i +
                            forces.i + avoidables_forces.i;
                        movable->coords_b.j = currentLocation.j + delta_j +
                            forces.j + avoidables_forces.j;
                    }
                }
                break;
            }
        }

        movables->use_coords_a = false;
        return bbSuccess;
    }
    else
    {
        for (I32 i = 0; i < NUM_MOVABLES; i++)
        {
            bbMovable* movable = &movables->movables[i];
            switch (movable->type)
            {
            case bbMovableType_Unused:
            case bbMovableType_Idle:
            case bbMovableType_Attacking:
            case bbMovableType_Dead:

                movable->coords_a.i = movable->coords_b.i;
                movable->coords_a.j = movable->coords_b.j;
                break;
            case bbMovableType_Moving:
                {
                    bbMilliCoords currentLocation = movable->coords_b;
                    bbMilliCoords goalPoint
                        = bbMapCoords_getMilliCoords(movable->goalpoint);


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(
                        distance_i * distance_i + distance_j * distance_j);

                    if (distance < movable->speed)
                    {
                        movable->coords_a = goalPoint;
                    }
                    else
                    {
                        double delta_i = distance_i / distance * movable->
                            speed;
                        double delta_j = distance_j / distance * movable->
                            speed;

                        bbMilliCoords forces = sumForces(movables, movable);
                        bbMilliCoords avoidables_forces =
                            bbAvoidables_sumForces(
                                home.agents_app.avoidables, movable);

                        movable->coords_a.i = currentLocation.i + delta_i +
                            forces.i + avoidables_forces.i;
                        movable->coords_a.j = currentLocation.j + delta_j +
                            forces.j + avoidables_forces.j;
                    }
                }
                break;

            case bbMovableType_MovingThrough:
                {
                    bbMilliCoords currentLocation = movable->coords_b;
                    bbMilliCoords goalPoint
                        = bbMapCoords_getMilliCoords(movable->goalpoint);


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(
                        distance_i * distance_i + distance_j * distance_j);

                    if (distance < movable->speed)
                    {
                        movable->coords_a = goalPoint;
                    }
                    else
                    {
                        double delta_i = distance_i / distance * movable->
                            speed;
                        double delta_j = distance_j / distance * movable->
                            speed;


                        movable->coords_a.i = currentLocation.i + delta_i;
                        movable->coords_a.j = currentLocation.j + delta_j;
                    }
                }
                break;
            case bbMovableType_Follow:
                {
                    movable->goalpoint = movables->movables[movable->
                        goal_movable].position;
                    bbMilliCoords currentLocation = movable->coords_b;
                    bbMilliCoords goalPoint
                        = bbMapCoords_getMilliCoords(movable->goalpoint);


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(
                        distance_i * distance_i + distance_j * distance_j);

                    if (distance < movable->speed)
                    {
                        movable->coords_a = goalPoint;
                    }
                    else
                    {
                        double delta_i = distance_i / distance * movable->
                            speed;
                        double delta_j = distance_j / distance * movable->
                            speed;

                        bbMilliCoords forces = sumForces(movables, movable);
                        bbMilliCoords avoidables_forces =
                            bbAvoidables_sumForces(
                                home.agents_app.avoidables, movable);

                        movable->coords_a.i = currentLocation.i + delta_i +
                            forces.i + avoidables_forces.i;
                        movable->coords_a.j = currentLocation.j + delta_j +
                            forces.j + avoidables_forces.j;
                    }
                }
                break;
            }
        }

        movables->use_coords_a = true;
        return bbSuccess;
    }
}


bbFlag bbMovables_update(bbMovables* movables)
{
    for (I32 i = 0; i < NUM_MOVABLES; i++)
    {
        //movables->movables[i].goalpoint = movables->movables[0].position;

        movables->movables[i].coords_a = bbMapCoords_getMilliCoords(
            movables->movables[i].position);
        movables->movables[i].coords_b = bbMapCoords_getMilliCoords(
            movables->movables[i].position);
    }

    for (I32 i = 0; i < movables->updates_per_frame; i++)
    {
        bbMovables_updateOnce(movables);
    }

    for (I32 i = 0; i < NUM_MOVABLES; i++)
    {
        if (movables->use_coords_a)
            movables->movables[i].position
                = bbMilliCoords_getMapCoords(movables->movables[i].coords_a);
        else
            movables->movables[i].position
                = bbMilliCoords_getMapCoords(movables->movables[i].coords_b);

        movables->movables[i].position.k
            = bbMapCoords_getElevation(&home.ground_surface,
                                       movables->movables[i].position);

        movables->buffer_back->movables[i].goalpoint = movables->movables[i]
            .goalpoint;
        movables->buffer_back->movables[i].position = movables->movables[i].
            position;
        movables->buffer_back->time = home.core.clock2_handle.map_tick;
    }
    if (home.core.core.simulation_time == home.core.core.actual_time)
    {
        bbMutexLock(&movables->buffer_mutex);

        bbMovables_snapshot* temp = movables->buffer_back;
        movables->buffer_back = movables->buffer_front;
        movables->buffer_front = temp;

        movables->buffer_fresh = true;

        bbMutexUnlock(&movables->buffer_mutex);
    }

    return bbSuccess;
}


bbFlag bbMovables_copyBuffer(bbMovables* movables,
                              bbMovables_snapshot* target)
{
    bbMutexLock(&movables->buffer_mutex);

    if (movables->buffer_fresh == true)
    {
        for (I32 i = 0; i < NUM_MOVABLES; i++)
        {
            target->movables[i].goalpoint = movables->buffer_front->movables[
                i].goalpoint;
            target->movables[i].position = movables->buffer_front->movables[
                i].position;
        }
        target->time = movables->buffer_front->time;
        movables->buffer_fresh = false;
    }
    bbMutexUnlock(&movables->buffer_mutex);


    return bbSuccess;
}


I32 bbMovables_newSkelly(bbMovables* movables, bbMapCoords position,
                          bbHandle agent)
{
    I32 index = movables->available++;
    bbMovable* movable = &movables->movables[index];
    movable->goalpoint = position;
    movable->position = position;
    movable->agent2 = agent;
    movable->type = bbMovableType_Idle;
    return index;
}
