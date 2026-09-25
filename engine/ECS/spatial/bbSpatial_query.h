#include "engine/ECS/spatial/bbSpatial.h"
#include "engine/logic/bbList.h"


typedef bbListFunction bbFilterFunction;

#define MAX_FILTER_FUNCTIONS 8

typedef struct
{
    bbMapCoords coords;
    U32 radius;
    bbListFunction* function;
    void* cl;

    bbFilterFunction* filters[MAX_FILTER_FUNCTIONS];
    void* cls[MAX_FILTER_FUNCTIONS];

}bbSpatialFilters;


bbFlag bbSpatial_mapRadius(bbSpatial* spatial,
                            bbMapCoords centre,
                            U64 radius_points,
                            bbListFunction* myFunc,
                            void* cl);

bbSpatialFilters bbSpatialFilters_new(bbMapCoords centre,
                                      U32 radius,
                                      bbListFunction*
                                      function,
                                      void* cl);

bbFlag bbSpatial_mapRadiusFilter(bbSpatial* spatial,
                            bbSpatialFilters* filters);



bbFlag bbFilter_canAttack_fn(bbList* list, void* node, void* cl);
bbFlag bbListFunction_findNearest_fn(bbList* list, void* node, void* cl);

bbFlag bbSpatial_findNearestTarget(bbCore* core, bbECS* ECS, bbHandle attacker_entity , bbHandle* target_entity, U32 max_distance);