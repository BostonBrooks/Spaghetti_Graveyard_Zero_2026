#include "engine/entities/bbEntities.h"
#include "engine/data/bbHome.h"

bbFlag bbEntities_init_core(bbEntities* entities)
{
    entities->available = 0;
    bbHandle null_agent;
    bbHandle null_movable;

    null_agent = home.agents_app.agents->pool->null;
    null_movable.u64 = UINT64_MAX;

    for (I32 i = 0; i < NUM_ENTITIES; i++)
    {
        entities->entity[i].agent = null_agent;
        entities->entity[i].movable = null_movable;
    }
    return bbSuccess;
}
bbFlag bbEntities_init_graphics(bbEntities* entities)
{

    entities->num_entities_graphics = 0;
    bbHandle null_unit;

    null_unit = home.viewport_app.units->pool->null;

    for (I32 i = 0; i < NUM_ENTITIES; i++)
    {
        entities->entity[i].unit = null_unit;
    }
    return bbSuccess;
}