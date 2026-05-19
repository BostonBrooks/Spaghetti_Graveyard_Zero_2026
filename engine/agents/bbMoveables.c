#include "engine/agents/bbMoveables.h"

#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"

#define ZOMBIE_SPEED    6000
#define SKELETON_SPEED  4000
bbMilliCoords getForce(bbMoveables* moveables, bbMoveable* moveableA, bbMoveable* moveableB)
{

    //SpriteUnits[subject].Forces.i += (10000*idist)/dist/(dist - footprint)/(dist - footprint);
    //from Spaghetti_Graveyard_Demos/OldNoTerrainDemo/06_Units.h

    bbMilliCoords coords_a, coords_b, output;

    if (moveables->use_coords_a)
    {
        coords_a = moveableA->coords_a;
        coords_b = moveableB->coords_a;
    } else
    {
        coords_a = moveableA->coords_b;
        coords_b = moveableB->coords_b;
    }


    double delta_i = (coords_a.i - coords_b.i);
    double delta_j = (coords_a.j - coords_b.j);
    double distance = sqrt(delta_i * delta_i + delta_j * delta_j);
    double distanceReduced = (distance - 0.2l*MILLS_PER_TILE)/10000.l;
    double distanceReduced2 = distance/100000.l;

    bbMilliCoords mC;
    mC.i = ((delta_i)/(distanceReduced2*distanceReduced*distanceReduced));
    mC.j = ((delta_j)/(distanceReduced2*distanceReduced*distanceReduced));
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

    for (I32 i=0; i < NUM_MOVEABLES; i++)
    {
        moveableB = &moveables->moveables[i];
        if (moveableB->type == bbMoveableType_Unused) continue;
        if (moveableA == moveableB) continue;
        single = getForce(moveables,moveableA, moveableB);
        total.i += single.i;
        total.j += single.j;

    }
    return total;
}


bbFlag bbMoveables_init(bbMoveables* moveables)
{
    moveables->updates_per_frame = 12;
    moveables->use_coords_a = true;
    moveables->available = 0;
    bbVPool_newBloated(&moveables->snapshots,sizeof(bbMoveables_snapshot),100,100,"bbMoveables_snapshot");

    moveables->buffer_back = &moveables->buffer_a;
    moveables->buffer_front = &moveables->buffer_b;
    moveables->buffer_fresh = true;

    pthread_mutex_init(&moveables->buffer_mutex,NULL);

    for (I32 i = 0; i < NUM_MOVEABLES; i++)
    {
        bbMoveable* moveable = &moveables->moveables[i];

        moveable->type = bbMoveableType_Unused;
    }
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

                moveable->coords_b.i = moveable->coords_a.i;
                moveable->coords_b.j = moveable->coords_a.j;
                    continue;
                case bbMoveableType_GoalPoint:
                    {
                        bbMilliCoords currentLocation = moveable->coords_a;
                        bbMilliCoords goalPoint
                           = bbMapCoords_getMilliCoords(moveable->goalpoint);


                        //TODO don't use floats or doubles
                        double distance_i = goalPoint.i - currentLocation.i;
                        double distance_j = goalPoint.j - currentLocation.j;

                        double distance = sqrt(distance_i * distance_i + distance_j * distance_j);

                        if (distance < ZOMBIE_SPEED)
                        {
                            moveable->coords_b = goalPoint;
                        } else
                        {
                            double delta_i = distance_i / distance * ZOMBIE_SPEED;
                            double delta_j = distance_j / distance * ZOMBIE_SPEED;


                            moveable->coords_b.i = currentLocation.i + delta_i;
                            moveable->coords_b.j = currentLocation.j + delta_j;

                        }
                           // moveable->position = moveable->coords_b;
                    }
                break;
                case bbMoveableType_GoalMoveable:
                   {
                    moveable->goalpoint = moveables->moveables[moveable->goal_moveable].position;
                        bbMilliCoords currentLocation = moveable->coords_a;
                        bbMilliCoords goalPoint
                           = bbMapCoords_getMilliCoords(moveable->goalpoint);


                        //TODO don't use floats or doubles
                        double distance_i = goalPoint.i - currentLocation.i;
                        double distance_j = goalPoint.j - currentLocation.j;

                        double distance = sqrt(distance_i * distance_i + distance_j * distance_j);

                        if (distance < SKELETON_SPEED)
                        {
                            moveable->coords_b = goalPoint;
                        } else
                        {
                            double delta_i = distance_i / distance * SKELETON_SPEED;
                            double delta_j = distance_j / distance * SKELETON_SPEED;

                            bbMilliCoords forces = sumForces(moveables, moveable);
                            bbMilliCoords avoidables_forces = bbAvoidables_sumForces (home.agents_app.avoidables, moveable);

                            moveable->coords_b.i = currentLocation.i + delta_i + forces.i+avoidables_forces.i;
                            moveable->coords_b.j = currentLocation.j + delta_j + forces.j+avoidables_forces.j;
                        }

                    }
                break;
            }

        }

        moveables->use_coords_a = false;
        return bbSuccess;
    } else {

        for (I32 i = 0; i < NUM_MOVEABLES; i++)
        {
            bbMoveable* moveable = &moveables->moveables[i];
            switch (moveable->type)
            {
            case bbMoveableType_Unused:
            case bbMoveableType_Idle:

                moveable->coords_a.i = moveable->coords_b.i;
                moveable->coords_a.j = moveable->coords_b.j;
                continue;
            case bbMoveableType_GoalPoint:
                {
                    bbMilliCoords currentLocation = moveable->coords_b;
                    bbMilliCoords goalPoint
                           = bbMapCoords_getMilliCoords(moveable->goalpoint);


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(distance_i * distance_i + distance_j * distance_j);

                    if (distance < ZOMBIE_SPEED)
                    {
                        moveable->coords_a = goalPoint;
                    } else
                    {
                        double delta_i = distance_i / distance * ZOMBIE_SPEED;
                        double delta_j = distance_j / distance * ZOMBIE_SPEED;


                        moveable->coords_a.i = currentLocation.i + delta_i;
                        moveable->coords_a.j = currentLocation.j + delta_j;
                    }

                }
                break;
            case bbMoveableType_GoalMoveable:
                {
                    moveable->goalpoint = moveables->moveables[moveable->goal_moveable].position;
                    bbMilliCoords currentLocation = moveable->coords_b;
                    bbMilliCoords goalPoint
                           = bbMapCoords_getMilliCoords(moveable->goalpoint);


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(distance_i * distance_i + distance_j * distance_j);

                    if (distance < SKELETON_SPEED)
                    {
                        moveable->coords_a = goalPoint;
                    } else
                    {
                        double delta_i = distance_i / distance * SKELETON_SPEED;
                        double delta_j = distance_j / distance * SKELETON_SPEED;

                        bbMilliCoords forces = sumForces(moveables, moveable);

                        moveable->coords_a.i = currentLocation.i + delta_i + forces.i;
                        moveable->coords_a.j = currentLocation.j + delta_j + forces.j;
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

        moveables->moveables[i].coords_a = bbMapCoords_getMilliCoords(moveables->moveables[i].position);
        moveables->moveables[i].coords_b = bbMapCoords_getMilliCoords(moveables->moveables[i].position);
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
            = bbMapCoords_getElevation(&home.ground_surface, moveables->moveables[i].position);

        moveables->buffer_back->moveables[i].goalpoint = moveables->moveables[i].goalpoint;
        moveables->buffer_back->moveables[i].position = moveables->moveables[i].position;
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


bbFlag bbMoveables_copyBuffer(bbMoveables* moveables, bbMoveables_snapshot* target)
{
    bbMutexLock(&moveables->buffer_mutex);

    if (moveables->buffer_fresh == true)
    {
        for (I32 i = 0; i < NUM_MOVEABLES; i++)
        {
            target->moveables[i].goalpoint = moveables->buffer_front->moveables[i].goalpoint;
            target->moveables[i].position = moveables->buffer_front->moveables[i].position;
        }
        target->time = moveables->buffer_front->time;
        moveables->buffer_fresh = false;
    }
    bbMutexUnlock(&moveables->buffer_mutex);



    return bbSuccess;
}


I32 bbMoveables_newSkelly(bbMoveables* moveables, bbMapCoords position, bbHandle agent)
{
    I32 index = moveables->available++;
    bbMoveable* moveable = &moveables->moveables[index];
    moveable->goalpoint = position;
    moveable->position = position;
    moveable->agent2 = agent;
    moveable->type = bbMoveableType_Idle;
    return index;
}
