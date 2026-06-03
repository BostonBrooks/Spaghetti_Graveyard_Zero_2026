#ifndef BB_SERVER_ENTITY_H
#define BB_SERVER_ENTITY_H

#include "engine/entities/bbMovables.h"
#include "engine/entities/bbEntities.h"

typedef struct
{
    bool in_use;
} bbServerEntity;

typedef struct
{
    bool in_use;
} bbServerMovable;

typedef struct
{
    I32 num_entities;
    I32 num_movables;
    bbServerEntity entity[NUM_ENTITIES];
    bbServerMovable movable[NUM_MovableS];

} bbServerEntities;

///Set in_use to false
bbFlag bbServerEntities_init(bbServerEntities *entities);


#endif   //BB_SERVER_ENTITY_H