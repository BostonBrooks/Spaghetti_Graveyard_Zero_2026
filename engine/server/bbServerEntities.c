

#include "engine/server/bbServerEntities.h"

#include "engine/entities/bbMovables.h"
#include "engine/entities/bbEntities.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbFlag.h"

bbFlag bbServerEntities_init(bbServerEntities *entities)
{
    entities->num_entities = 0;
    entities->num_movables = 0;
    for (I32 i = 0; i<NUM_ENTITIES;i++)
    {
        entities->entity[i].in_use = false;
    }

    for (I32 i = 0; i<NUM_MovableS;i++)
    {
        entities->movable[i].in_use = false;
    }

    return bbSuccess;
}

bbFlag bbServerEntities_load(bbServerEntities *entities, char* filename)
{
    bbHere()
}

