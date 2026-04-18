#include "engine/agents/bbMoveables.h"

#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"


bbFlag bbMoveables_init(bbMoveables* moveables)
{
    moveables->updatesPerFrame = 8;
    moveables->useCoordsA = true;

    bbVPool_newBloated(&moveables->snapshots,sizeof(bbMoveables_snapshot),100,100);

    for (I32 i = 0; i < numMoveables; i++)
    {
        bbMoveable* moveable = &moveables->moveables[i];

        moveable->type = bbMoveableType_Unused;

        moveable->coords_original.i = 0;
        moveable->coords_original.j = 0;
        moveable->coords_original.k = 0;

        moveable->coordsA.i = 0;
        moveable->coordsA.j = 0;
        moveable->coordsA.k = 0;

        moveable->coordsB.i = 0;
        moveable->coordsB.j = 0;
        moveable->coordsB.k = 0;

        moveable->goal_point.i = 0;
        moveable->goal_point.j = 0;
        moveable->goal_point.k = 0;



    }

    moveables->moveables[0].type = bbMoveableType_Player;
    moveables->moveables[0].goal_point.i = 10000000;
    moveables->moveables[0].goal_point.j = 10000000;
    moveables->moveables[0].goal_point.k = 10000000;
    return bbSuccess;
}

bbFlag bbMoveables_updateOnce(bbMoveables* moveables)
{
    //for now, just move 2048 mills toward goal point
    if (moveables->useCoordsA)
    {
        for (I32 i = 0; i < numMoveables; i++)
        {
            bbMoveable* moveable = &moveables->moveables[i];
            switch (moveable->type)
            {
                case bbMoveableType_Unused:

                moveable->coordsB.i = moveable->coordsA.i;
                moveable->coordsB.j = moveable->coordsA.j;
                    continue;
                case bbMoveableType_Player:
                    {
                        bbMilliCoords currentLocation = moveable->coordsA;
                        bbMilliCoords goalPoint = moveable->goal_point;


                        //TODO don't use floats or doubles
                        double distance_i = goalPoint.i - currentLocation.i;
                        double distance_j = goalPoint.j - currentLocation.j;

                        double distance = sqrt(distance_i * distance_i + distance_j * distance_j);

                        if (distance < 4048)
                        {
                            moveable->coordsB = goalPoint;
                        } else
                        {
                            double delta_i = distance_i / distance * 4048;
                            double delta_j = distance_j / distance * 4048;


                            moveable->coordsB.i = currentLocation.i + delta_i;
                            moveable->coordsB.j = currentLocation.j + delta_j;
                        }
                    }
            }

        }

        moveables->useCoordsA = false;
        return bbSuccess;
    } else {

        for (I32 i = 0; i < numMoveables; i++)
        {
            bbMoveable* moveable = &moveables->moveables[i];
            switch (moveable->type)
            {
            case bbMoveableType_Unused:

                moveable->coordsA.i = moveable->coordsB.i;
                moveable->coordsA.j = moveable->coordsB.j;
                continue;
            case bbMoveableType_Player:
                {
                    bbMilliCoords currentLocation = moveable->coordsB;
                    bbMilliCoords goalPoint = moveable->goal_point;


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(distance_i * distance_i + distance_j * distance_j);

                    if (distance < 4048)
                    {
                        moveable->coordsA = goalPoint;
                    } else
                    {
                        double delta_i = distance_i / distance * 4048;
                        double delta_j = distance_j / distance * 4048;


                        moveable->coordsA.i = currentLocation.i + delta_i;
                        moveable->coordsA.j = currentLocation.j + delta_j;
                    }
                }
            }

        }

        moveables->useCoordsA = true;
        return bbSuccess;
    }
}

bbFlag bbMoveables_update(bbMoveables* moveables)
{



    bbMapCoords goalPoint = home.core.goalpoint;

    bbMilliCoords goalCoords = bbMapCoords_getMilliCoords(goalPoint);

    moveables->moveables[0].goal_point = goalCoords;

    for (I32 i = 0; i < moveables->updatesPerFrame; i++)
    {
        bbMoveables_updateOnce(moveables);
    }
    bbMapCoords MC;
    if (moveables->useCoordsA)
        MC = bbMilliCoords_getMapCoords(moveables->moveables[0].coordsA);
    else MC = bbMilliCoords_getMapCoords(moveables->moveables[0].coordsB);

    home.viewport_app.viewport.viewpoint = MC;
    return bbSuccess;
}