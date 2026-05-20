#ifndef BB_SERVER_ENTITY_H
#define BB_SERVER_ENTITY_H

#include "engine/agents/bbMoveables.h"
#include "engine/data/bbEntities.h"

typedef struct
{
    bool in_use;
} bbServerEntity;

typedef struct
{
    bool in_use;
} bbServerMoveable;

typedef struct
{
    I32 num_entities;
    I32 num_moveables;
    bbServerEntity entity[NUM_ENTITIES];
    bbServerMoveable moveable[NUM_MOVEABLES];

} bbServerEntities;

///Set in_use to false
bbFlag bbServerEntities_init(bbServerEntities *entities);


#endif   //BB_SERVER_ENTITY_H