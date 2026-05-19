

#include "engine/server/bbServerEntities.h"

#include "engine/agents/bbMoveables.h"
#include "engine/data/bbEntities.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbFlag.h"

bbFlag bbServerEntities_init(bbServerEntities *entities)
{
    entities->num_entities = 0;
    for (I32 i = 0; i<NUM_ENTITIES;i++)
    {
        entities->entity[i].in_use = false;
    }

    for (I32 i = 0; i<NUM_MOVEABLES;i++)
    {
        entities->moveable[i].in_use = false;
    }

    return bbSuccess;
}

bbFlag bbServerEntities_load(bbServerEntities *entities, char* filename)
{
    bbHere()
}

