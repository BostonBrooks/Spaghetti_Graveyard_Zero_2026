#ifndef BB_ENTITIES_H
#define BB_ENTITIES_H

#define  NUM_ENTITIES 1024
#include "bbMovables.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

typedef struct
{
    bbHandle agent;
    bbHandle movable;
    bbHandle unit;
}bbEntity;

typedef struct
{
    I32 available;
    I32 num_entities_graphics;
    bbEntity entity[NUM_ENTITIES];

    ///Used so that movables can send updates to units
    bbHandle movable_units[NUM_MOVABLES];
}bbEntities;

bbFlag bbEntities_init_core(bbEntities* entities);
bbFlag bbEntities_init_graphics(bbEntities* entities);
#endif //BB_ENTITIES_H