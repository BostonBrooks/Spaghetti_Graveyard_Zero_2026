#include "engine/agents/bbMoveables.h"

#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"


bbFlag bbMoveables_init(bbMoveables* moveables)
{
    moveables->updates_per_frame = 8;
    moveables->use_coords_a = true;

    bbVPool_newBloated(&moveables->snapshots,sizeof(bbMoveables_snapshot),10,10);

    moveables->buffer_back = &moveables->buffer_a;
    moveables->buffer_front = &moveables->buffer_b;

    pthread_mutex_init(&moveables->buffer_mutex,NULL);

    for (I32 i = 0; i < numMoveables; i++)
    {
        bbMoveable* moveable = &moveables->moveables[i];

        moveable->type = bbMoveableType_Unused;

        moveable->position.i = 0;
        moveable->position.j = 0;
        moveable->position.k = 0;

        moveable->coords_a.i = 0;
        moveable->coords_a.j = 0;
        moveable->coords_a.k = 0;

        moveable->coords_b.i = 0;
        moveable->coords_b.j = 0;
        moveable->coords_b.k = 0;

        moveable->goalpoint.i = 0;
        moveable->goalpoint.j = 0;
        moveable->goalpoint.k = 0;



    }

    moveables->moveables[0].type = bbMoveableType_Player;
    moveables->moveables[0].goalpoint.i = 10000000;
    moveables->moveables[0].goalpoint.j = 10000000;
    moveables->moveables[0].goalpoint.k = 10000000;
    return bbSuccess;
}

bbFlag bbMoveables_updateOnce(bbMoveables* moveables)
{
    //for now, just move 2048 mills toward goal point
    if (moveables->use_coords_a)
    {
        for (I32 i = 0; i < numMoveables; i++)
        {
            bbMoveable* moveable = &moveables->moveables[i];
            switch (moveable->type)
            {
                case bbMoveableType_Unused:

                moveable->coords_b.i = moveable->coords_a.i;
                moveable->coords_b.j = moveable->coords_a.j;
                    continue;
                case bbMoveableType_Player:
                    {
                        bbMilliCoords currentLocation = moveable->coords_a;
                        bbMilliCoords goalPoint = moveable->goalpoint;


                        //TODO don't use floats or doubles
                        double distance_i = goalPoint.i - currentLocation.i;
                        double distance_j = goalPoint.j - currentLocation.j;

                        double distance = sqrt(distance_i * distance_i + distance_j * distance_j);

                        if (distance < 4048)
                        {
                            moveable->coords_b = goalPoint;
                        } else
                        {
                            double delta_i = distance_i / distance * 4048;
                            double delta_j = distance_j / distance * 4048;


                            moveable->coords_b.i = currentLocation.i + delta_i;
                            moveable->coords_b.j = currentLocation.j + delta_j;
                        }
                    }
            }

        }

        moveables->use_coords_a = false;
        return bbSuccess;
    } else {

        for (I32 i = 0; i < numMoveables; i++)
        {
            bbMoveable* moveable = &moveables->moveables[i];
            switch (moveable->type)
            {
            case bbMoveableType_Unused:

                moveable->coords_a.i = moveable->coords_b.i;
                moveable->coords_a.j = moveable->coords_b.j;
                continue;
            case bbMoveableType_Player:
                {
                    bbMilliCoords currentLocation = moveable->coords_b;
                    bbMilliCoords goalPoint = moveable->goalpoint;


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(distance_i * distance_i + distance_j * distance_j);

                    if (distance < 4048)
                    {
                        moveable->coords_a = goalPoint;
                    } else
                    {
                        double delta_i = distance_i / distance * 4048;
                        double delta_j = distance_j / distance * 4048;


                        moveable->coords_a.i = currentLocation.i + delta_i;
                        moveable->coords_a.j = currentLocation.j + delta_j;
                    }
                }
            }

        }

        moveables->use_coords_a = true;
        return bbSuccess;
    }
}

bbFlag bbMoveables_update(bbMoveables* moveables)
{



    bbMapCoords goalPoint = home.core.goalpoint;

    bbMilliCoords goalCoords = bbMapCoords_getMilliCoords(goalPoint);

    moveables->moveables[0].goalpoint = goalCoords;

    for (I32 i = 0; i < moveables->updates_per_frame; i++)
    {
        bbMoveables_updateOnce(moveables);
    }

    for (I32 i = 0; i < numMoveables; i++)
    {
        if (moveables->use_coords_a)
            moveables->moveables[i].position = moveables->moveables[i].coords_a;
        else
            moveables->moveables[i].position = moveables->moveables[i].coords_b;


        moveables->buffer_back->moveables[i].goalpoint = moveables->moveables[i].goalpoint;
        moveables->buffer_back->moveables[i].position = moveables->moveables[i].position;
    }

    bbMutexLock(&moveables->buffer_mutex);

    bbMoveables_snapshot* temp = moveables->buffer_back;
    moveables->buffer_back = moveables->buffer_front;
    moveables->buffer_front = temp;

    bbMutexUnlock(&moveables->buffer_mutex);



    return bbSuccess;
}


bbFlag bbMoveables_copyBuffer(bbMoveables* moveables, bbMoveables_snapshot* target)
{
    bbMutexLock(&moveables->buffer_mutex);


    for (I32 i = 0; i < numMoveables; i++)
    {
        target->moveables[i].goalpoint = moveables->moveables[i].goalpoint;
        target->moveables[i].position = moveables->moveables[i].position;
    }

    bbMutexUnlock(&moveables->buffer_mutex);

    bbMapCoords MC = bbMilliCoords_getMapCoords(target->moveables[0].position);

    home.viewport_app.viewport.viewpoint = MC;;

    return bbSuccess;
}