#ifndef BB_ECS_H
#define BB_ECS_H

#include "engine/core/bbInstruction.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbList.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbTerminal.h"

typedef enum
{
    bbECS_Moveables,
    bbECS_AI,
    bbECS_Unit,
    bbECS_ServerEntities,
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
    char key[KEY_LENGTH];
    bbListElement_Handle list_element_handle;
    bbECS_states state;
    U64 has_component;
    bbHandle components[bbECS_numSystems];
} bbECS_entity;

/// Contains a minimal amount of data to make systems somewhat interchangeable
typedef struct
{
    bbVPool* pool;
} bbSystem;

typedef struct
{
    bbVPool* pool;
    bbList list;
    bbSystem systems[bbECS_numSystems];
} bbECS;



bbFlag bbECS_init(bbECS* ECS);

/// This function is called by Entity spawner instructions
bbFlag bbECS_newEntity(bbECS* ECS, bbHandle* handle, bbECS_entity** entity);
/// This function is called by instructions that are each called by
/// Entity spawner functions
bbFlag bbECS_entity_setComponent(bbECS* ECS, bbHandle entity, bbHandle component, bbECS_systems system);


bbFlag bbCoreInput_spawnTestEntity(bbCore, bbECS* ECS, char* KEY);
bbFlag bbInstruction_spawnTestEntity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unspawnTestEntity_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbCoreIntput_entity_setComponent(core* core, bbHandle entity, bbHandle component, bbECS_systems system);
bbFlag bbInstruction_entity_setComponent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_entity_unsetComponent_fn(bbCore* core, bbInstruction* instruction);
#endif //BB_ECS_H