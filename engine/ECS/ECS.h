#include "engine/logic/bbHandle.h"
#include "engine/logic/bbList.h"
#include "engine/logic/bbVPool.h"

typedef enum
{
    bbECS_Moveables,
    bbECS_AI,
    bbECS_Unit,
    bbECS_numSystems
} bbECS_systems;

typedef enum
{
    bbECS_unused,
    bbECS_alive,
    bbECS_dying,
    bbECS_dead,
} bbECS_states;

typedef struct
{
    bbListElement_Handle list_element_handle;
    bbECS_states state;
    U64 has_component;
    bbHandle components[bbECS_numSystems];
} bbECS_entity;

typedef struct
{
    bbVPool* pool;
    bbList list;
} bbECS;

bbFlag bbECS_init(bbECS* ECS);
bbFlag bbECS_entity_getComponent(bbHandle* component, bbHandle entity, bbECS_systems system);
bbFlag bbECS_entity_setComponent(bbHandle component, bbHandle entity, bbECS_systems system);