#ifndef BB_ENTITIES_H
#define BB_ENTITIES_H

#define  NUM_ENTITIES 1024
#include "bbMoveables.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

typedef struct
{
    bbHandle agent;
    bbHandle moveable;
    bbHandle unit;
}bbEntity;

typedef struct
{
    I32 available;
    I32 num_entities_graphics;
    bbEntity entity[NUM_ENTITIES];

    ///Used so that moveables can send updates to units
    bbHandle moveable_units[NUM_MOVEABLES];
}bbEntities;

bbFlag bbEntities_init_core(bbEntities* entities);
bbFlag bbEntities_init_graphics(bbEntities* entities);
#endif //BB_ENTITIES_H