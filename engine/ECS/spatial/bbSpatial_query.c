#include "engine/ECS/spatial/bbSpatial_query.h"
#include "engine/ECS/spatial/bbSpatial.h"
#include "engine/logic/bbIterator.h"

//typedef bbFlag bbListFunction(bbList* list, void* node, void* cl);
typedef struct
{
    bbMapCoords coords;
    U64 radius;
    bbListFunction* function;
    void* cl;
}bbSpatial_queryRadius_cl;

bbFlag bbListFunction_queryRadius(bbList* list, void* node, void* cl)
{
    bbSpatial_queryRadius_cl* query = cl;
    bbSpatial_Component* component = node;
    I64 delta_i = component->map_coords.i - query->coords.i;
    I64 delta_j = component->map_coords.j - query->coords.j;

    if (delta_i * delta_i + delta_j * delta_j > query->radius * query->radius) return bbContinue;

    return query->function(list, node, query->cl);
}

bbFlag bbSpatial_mapRadius(bbSpatial* spatial,
                            bbMapCoords centre,
                            U64 radius_points,
                            bbListFunction* myFunc,
                            void* cl)
{
    bbMapCoords left = centre;
    left.i -= radius_points;
    left.j -= radius_points;

    bbMapCoords right = centre;
    right.i += radius_points;
    right.j += radius_points;

    bbSquareCoords left_square = bbMapCoords_getSquareCoords(left);
    bbSquareCoords right_square = bbMapCoords_getSquareCoords(right);

    // bbDebug("left.i = %d, left.j = %d, right.i = %d, right.j = %d\n",
    //     left_square.i, left_square.j, right_square.i, right_square.j);

    bbSpatial_queryRadius_cl query;
    query.radius = radius_points;
    query.function = myFunc;
    query.coords = centre;
    query.cl = cl;

    for (I32 i = left_square.i; i <= right_square.i; i++)
    {
        for (I32 j = left_square.j; j <= right_square.j; j++)
        {
            I32 index = bbSpatial_getSquareIndex(i, j, spatial->squares_i, spatial->squares_j);
            if (index < 0) continue;
            bbSpatialSquare* square = &spatial->squares[index];
            bbIterator iterator = bbIterator_new(&square->list);
            bbFlag flag = bbIterator_mapL(&iterator, bbListFunction_queryRadius, &query);
            //TODO process flag
            //bbDebug("i = %d, j = %d\n", i, j);
        }
    }
    return bbSuccess;
}