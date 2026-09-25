#include "engine/ECS/spatial/bbSpatial_query.h"
#include "engine/ECS/spatial/bbSpatial.h"
#include "engine/ECS/teams/bbTeams.h"
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
        if (flag == bbContinue) {
            //bbHere()
            return bbContinue;
        }
        if (flag == bbSuccess) {
            //bbHere()
            continue;
        }
        bbFlag_print(flag);
    }

    bbFilterFunction* function = query->function;
    return function(list, node, query->cl);
}


///spatial query and filter before passing to bbListFuction
bbFlag bbSpatial_mapRadiusFilter(bbSpatial* spatial,
                            bbSpatialFilters* query)
{
    //bbDebug("radius = %llu\n", query->radius);
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

            //bbDebug("square i = %d, square_j = %d, index = %d, radius = %d\n", i, j, index, query->radius);
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

typedef struct
{
    bbECS* ECS;
    bbHandle nearest_entity;
    bbHandle attacker_entity;
    U32 nearest_distance;
} bbFilter_findNearest_cl;

///Find the nearest target
bbFlag bbListFunction_findNearest_fn(bbList* list, void* node, void* cl)
{
    bbFilter_findNearest_cl* filter_cl = cl;
    bbSpatial_Component* component = node;

    bbHandle target_entity_handle = component->component.entity_handle;
    bbMapCoords target_coords = component->map_coords;
    bbHandle attacker_entity_handle = filter_cl->attacker_entity;
    bbHandle attacker_spatial_handle;
    bbSpatial_Component* attacker_spatial;
    bbHandle_mapComponent(filter_cl->ECS,
        bbECS_ECS,
        attacker_entity_handle,
        bbECS_Spatial,
        &attacker_spatial_handle,
        (bbComponent**)&attacker_spatial);

    I64 delta_i = target_coords.i - attacker_spatial->map_coords.i;
    I64 delta_j = target_coords.j - attacker_spatial->map_coords.j;
    I64 dist_squared = delta_i * delta_i + delta_j * delta_j;
    I64 distance = bbArith64_sqrt2(dist_squared);

    if (bbHandleError_NULL == bbVPool_handleIsNULL(filter_cl->ECS->systems[bbECS_ECS]->pool,filter_cl->nearest_entity))
    {


        filter_cl->nearest_distance = distance;
        filter_cl->nearest_entity = target_entity_handle;
        return bbContinue;
    }

    if (distance < filter_cl->nearest_distance)
    {
        filter_cl->nearest_distance = distance;
        filter_cl->nearest_entity = target_entity_handle;
        return bbContinue;
    }

    if (distance > filter_cl->nearest_distance) return bbContinue;

    if (delta_i < 0)
    {
        filter_cl->nearest_distance = distance;
        filter_cl->nearest_entity = target_entity_handle;
        return bbContinue;
    }

    if (delta_i > 0) return bbContinue;

    if (delta_j < 0)
    {
        filter_cl->nearest_distance = distance;
        filter_cl->nearest_entity = target_entity_handle;
        return bbContinue;
    }

    if (delta_j > 0) return bbContinue;

    bbNotHere()
    return bbContinue;
}

typedef struct
{
    bbECS* ECS;
    bbHandle attacker_entity;
} bbFilter_canAttack_cl;


bbFlag bbFilter_canAttack_fn(bbList* list, void* node, void* cl)
{
    bbFilter_canAttack_cl* filter_cl = cl;
    bbSpatial_Component* component = node;
    bbHandle target_entity_handle = component->component.entity_handle;
    bbHandle attacker_entity_handle = filter_cl->attacker_entity;
    bbFlag flag;
    bbTeam* attacker_team;
    flag = bbHandle_mapComponent(filter_cl->ECS,
                                 bbECS_ECS,
                                 attacker_entity_handle,
                                 bbECS_Teams,
                                 NULL,
                                 (bbComponent**)&attacker_team);
    if (flag == bbNone) {
        //bbHere()
        return bbContinue;
    }
    if (attacker_team == NULL) {
        //bbHere()
        return bbContinue;
    }
    bbTeam* target_team;
    flag = bbHandle_mapComponent(filter_cl->ECS,
                                 bbECS_ECS,
                                 target_entity_handle,
                                 bbECS_Teams,
                                 NULL,
                                 (bbComponent**)&target_team);
    if (flag == bbNone) {
        //bbHere()
        return bbContinue;
    }
    if (target_team == NULL) {
        //bbHere()
        return bbContinue;
    }

    if (attacker_team->team != target_team->team) {
        return(bbSuccess);
    }

    return bbContinue;
}

bbFlag bbSpatial_findNearestTarget(bbCore* core,
                                   bbECS* ECS,
                                   bbHandle attacker_entity,
                                   bbHandle* target_entity,
                                   U32 max_distance)
{
    bbFilter_findNearest_cl nearest_cl;

    nearest_cl.ECS = ECS;
    nearest_cl.nearest_entity = *target_entity;
    nearest_cl.attacker_entity = attacker_entity;
    nearest_cl.nearest_distance = max_distance+193;

    bbFilter_canAttack_cl can_attack_cl;
    can_attack_cl.attacker_entity = attacker_entity;
    can_attack_cl.ECS = ECS;

    bbSpatial_Component* attacker_spatial;
    bbHandle_mapComponent(ECS,
                   bbECS_ECS,
                   attacker_entity,
                   bbECS_Spatial,
                   NULL,
                   (bbComponent**)&attacker_spatial);

    bbSpatialFilters filters_cl;


    for (I32 i=0; i<MAX_FILTER_FUNCTIONS; i++)
    {
        filters_cl.filters[i] = NULL;
        filters_cl.cls[i] = NULL;
    }
    filters_cl.filters[0] = bbFilter_canAttack_fn;
    filters_cl.cls[0] = &can_attack_cl;

    filters_cl.coords = attacker_spatial->map_coords;
    filters_cl.radius = max_distance;
    filters_cl.function = bbListFunction_findNearest_fn;
    filters_cl.cl = &nearest_cl;

    bbSpatial_mapRadiusFilter((bbSpatial*)ECS->systems[bbECS_Spatial],&filters_cl);

    bbHandle nearest_entity_handle = nearest_cl.nearest_entity;
    I64 nearest_distance = nearest_cl.nearest_distance;

    if (bbSuccess != bbVPool_handleIsNULL(ECS->system.pool,nearest_entity_handle))
    {
        return bbFail;
    }
    if (nearest_distance == max_distance+193)
    {
        return bbFail;
    }

    *target_entity = nearest_entity_handle;
    return bbSuccess;
}