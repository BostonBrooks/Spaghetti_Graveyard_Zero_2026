#include "engine/agents/bbMoveables.h"

#include "engine/data/bbHome.h"


bbFlag bbMoveables_init(bbMoveables* moveables)
{
    moveables->updatesPerFrame = 8;
    moveables->useCoordsA = true;
    for (I32 i = 0; i < numMoveables; i++)
    {
        bbMoveable* moveable = &moveables->moveables[i];

        moveable->type = bbMoveableType_Unused;

        moveable->coordsOriginal.i = 0;
        moveable->coordsOriginal.j = 0;
        moveable->coordsOriginal.k = 0;

        moveable->coordsA.i = 0;
        moveable->coordsA.j = 0;
        moveable->coordsA.k = 0;

        moveable->coordsB.i = 0;
        moveable->coordsB.j = 0;
        moveable->coordsB.k = 0;

        moveable->goalPoint.i = 0;
        moveable->goalPoint.j = 0;
        moveable->goalPoint.k = 0;



    }

    moveables->moveables[0].type = bbMoveableType_Player;
    moveables->moveables[0].goalPoint.i = 10000000;
    moveables->moveables[0].goalPoint.j = 10000000;
    moveables->moveables[0].goalPoint.k = 10000000;
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
                    continue;
                case bbMoveableType_Player:
                    {
                        bbMilliCoords currentLocation = moveable->coordsA;
                        bbMilliCoords goalPoint = moveable->goalPoint;


                        //TODO don't use floats or doubles
                        double distance_i = goalPoint.i - currentLocation.i;
                        double distance_j = goalPoint.j - currentLocation.j;

                        double distance = sqrt(distance_i * distance_i + distance_j * distance_j);

                        if (distance < 2048)
                        {
                            moveable->coordsB = goalPoint;
                        } else
                        {
                            double delta_i = distance_i / distance * 2048;
                            double delta_j = distance_j / distance * 2048;


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
                continue;
            case bbMoveableType_Player:
                {
                    bbMilliCoords currentLocation = moveable->coordsB;
                    bbMilliCoords goalPoint = moveable->goalPoint;


                    //TODO don't use floats or doubles
                    double distance_i = goalPoint.i - currentLocation.i;
                    double distance_j = goalPoint.j - currentLocation.j;

                    double distance = sqrt(distance_i * distance_i + distance_j * distance_j);

                    if (distance < 2048)
                    {
                        moveable->coordsA = goalPoint;
                    } else
                    {
                        double delta_i = distance_i / distance * 2048;
                        double delta_j = distance_j / distance * 2048;


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
    for (I32 i = 0; i < moveables->updatesPerFrame; i++)
    {
        bbMoveables_updateOnce(moveables);
    }

    bbMapCoords MC = bbMilliCoords_getMapCoords(moveables->moveables[0].coordsA);
    home.viewport_app.viewport.viewpoint = MC;
    return bbSuccess;
}