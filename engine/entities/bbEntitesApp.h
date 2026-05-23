
#include "engine/agents/bbEntities.h"
#include "engine/agents/bbMoveables.h"

typedef struct
{
    bbHandle agent;
    bbHandle moveable;
    bbHandle unit;
}bbEntity;

typedef struct
{

    ///map from moveable index to unit handle
    bbHandle moveable_units[NUM_MOVEABLES];

    bbEntity entity[NUM_ENTITIES];

}bbEntities_arrays;



typedef struct
{
    bbEntities_arrays arrays;
} bbEntitiesApp;