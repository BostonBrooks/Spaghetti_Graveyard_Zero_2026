#include "engine/ECS/spatial/bbSpatial.h"
#include "engine/logic/bbList.h"
bbFlag bbSpatial_mapRadius(bbSpatial* spatial,
                            bbMapCoords centre,
                            U64 radius_points,
                            bbListFunction* myFunc,
                            void* cl);

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

bbSpatialFilters bbSpatialFilters_new(bbMapCoords centre,
                                      U32 radius,
                                      bbListFunction*
                                      function,
                                      void* cl);

bbFlag bbSpatial_mapRadiusFilter(bbSpatial* spatial,
                            bbSpatialFilters* filters);



