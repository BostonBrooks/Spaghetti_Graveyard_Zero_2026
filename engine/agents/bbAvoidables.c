#include "engine/agents/bbAvoidables.h"

#include "bbMoveables.h"
#include "engine/data/bbConstants.h"
#include "engine/logic/bbBloatedPool.h"

bbMilliCoords bbAvoidable_getForce(bbMoveables* moveables, bbMoveable* moveableA, bbAvoidable* avoidable)
{

    //SpriteUnits[subject].Forces.i += (10000*idist)/dist/(dist - footprint)/(dist - footprint);
    //from Spaghetti_Graveyard_Demos/OldNoTerrainDemo/06_Units.h

    bbMilliCoords coords_a, coords_b, output;

    if (moveables->use_coords_a)
    {
        coords_a = moveableA->coords_a;
    } else
    {
        coords_a = moveableA->coords_b;
    }

    coords_b = bbMapCoords_getMilliCoords(avoidable->coords);

    double delta_i = (coords_a.i - coords_b.i);
    double delta_j = (coords_a.j - coords_b.j);
    double distance = sqrt(delta_i * delta_i + delta_j * delta_j);
    double distanceReduced = (distance - 0.5l*MILLS_PER_TILE)/10000.l;
    double distanceReduced2 = distance/100000.l;

    bbMilliCoords mC;
    mC.i = ((delta_i)/(distanceReduced2*distanceReduced*distanceReduced));
    mC.j = ((delta_j)/(distanceReduced2*distanceReduced*distanceReduced));
    mC.k = 0;

    return mC;
}


bbFlag bbAvoidables_new(bbAvoidables** self, I32 squares_i, I32 squares_j)
{
    bbAvoidables* avoidables = malloc (sizeof(bbAvoidables) + squares_i*squares_j*sizeof(bbAvoidableSquare));
    avoidables->size.i = squares_i;
    avoidables->size.j = squares_j;

    bbVPool_newBloated(&avoidables->pool, sizeof(bbAvoidable),100,100);

    for (I32 i = 0; i < avoidables->size.i; i++)
    {
        for (I32 j = 0; j < avoidables->size.j; j++)
        {
            bbAvoidableSquare* square;
            square = &avoidables->squares[i*avoidables->size.j + j];
            square->coords.i = i;
            square->coords.j = j;

            bbList_init(&square->list, avoidables->pool, NULL,offsetof
            (bbAvoidable, listElement),NULL);

        }
    }
    *self = avoidables;
    return bbSuccess;
}


bbFlag bbAvoidable_newCircle(bbAvoidables* avoidables, bbMapCoords MC, I32 radius)
{
    bbVPool* pool = avoidables->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    I32 index = SC.i*avoidables->size.j + SC.j;
    bbAvoidableSquare* square = &avoidables->squares[index];

    bbAvoidable* avoidable;
    bbVPool_alloc(pool, (void**)&avoidable);
    avoidable->coords = MC;
    avoidable->radius = radius;
    avoidable->shape = bbAvoidableShape_Circle;

    bbList_pushL(&square->list, (void*)avoidable);

    return bbSuccess;
}