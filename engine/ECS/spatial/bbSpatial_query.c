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

            //bbDebug("i = %d, j = %d\n", i, j);
            bbFlag flag = bbIterator_mapL(&iterator, bbListFunction_queryRadius, &query);
            //TODO process flag
        }
    }
    return bbSuccess;
}

bbFlag bbListFunction_queryRadiusFilter(bbList* list, void* node, void* cl)
{//bbHere()
    bbSpatialFilters* query = cl;
    bbSpatial_Component* component = node;
    I64 delta_i = component->map_coords.i - query->coords.i;
    I64 delta_j = component->map_coords.j - query->coords.j;
    I64 radius = query->radius;
     //   bbDebug("distance = %llu, radius = %llu\n", bbArith64_sqrt2(delta_i * delta_i + delta_j * delta_j),radius)
    if (delta_i * delta_i + delta_j * delta_j > radius * radius) {

        return bbContinue;
    }
    for (I32 i = 0; i < MAX_FILTER_FUNCTIONS; i++) {

        bbFilterFunction* function = query->filters[i];
        if (function == NULL) {
            //bbHere()
            continue;
        }
        bbFlag flag = function(list, node, query->cls[i]);
        if (flag == bbContinue) {bbHere() return bbContinue;}
        if (flag == bbSuccess)  {bbHere() continue;}
        bbFlag_print(flag);
    }

    bbFilterFunction* function = query->function;
    return function(list, node, query->cl);
}


///spatial query and filter before passing to bbListFuction
bbFlag bbSpatial_mapRadiusFilter(bbSpatial* spatial,
                            bbSpatialFilters* query)
{
    bbDebug("radius = %llu\n", query->radius);
    bbMapCoords left = query->coords;
    left.i -= query->radius;
    left.j -= query->radius;

    bbMapCoords right = query->coords;
    right.i += query->radius;
    right.j += query->radius;


    bbSquareCoords left_square = bbMapCoords_getSquareCoords(left);
    bbSquareCoords right_square = bbMapCoords_getSquareCoords(right);



    for (I32 i = left_square.i; i <= right_square.i; i++)
    {
        for (I32 j = left_square.j; j <= right_square.j; j++)
        {
            I32 index = bbSpatial_getSquareIndex(i, j, spatial->squares_i, spatial->squares_j);

            bbDebug("square i = %d, square_j = %d, index = %d, radius = %d\n", i, j, index, query->radius);
            if (index < 0) continue;
            bbSpatialSquare* square = &spatial->squares[index];
            bbIterator iterator = bbIterator_new(&square->list);

            //bbDebug("radius = %llu\n", query->radius);
            //bbDebug("i = %d, j = %d\n", i, j);
            bbFlag flag = bbIterator_mapL(&iterator, bbListFunction_queryRadiusFilter, query);
            //TODO process flag
        }
    }
    return bbSuccess;
}

bbSpatialFilters bbSpatialFilters_new(bbMapCoords centre,
                                      U32 radius,
                                      bbListFunction* function,
                                      void* cl)
{
    bbSpatialFilters spatial_filters;

    spatial_filters.coords = centre;
    spatial_filters.radius = radius;
    spatial_filters.function = function;
    spatial_filters.cl = cl;

    for (I32 i = 0; i < MAX_FILTER_FUNCTIONS; i++) {
        spatial_filters.filters[i] = NULL;
        spatial_filters.cls[i] = NULL;
    }
    return spatial_filters;
}