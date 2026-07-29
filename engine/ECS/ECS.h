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
    bbECS_Graphics,
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
/// Like a base-class for systems
typedef struct
{
    bbVPool* pool;
} bbSystem;

typedef struct
{
    bbVPool* pool;
    bbList list;
    bbSystem* systems[bbECS_numSystems];
} bbECS;



bbFlag bbECS_init(bbECS* ECS);

//We need a function that spawns an entity, readies the unspawn instruction, then synchronously returns the entity.
bbFlag bbCoreSynchronous_spawnEmptyEntity(bbCore* core,
                                          bbECS* ECS,
                                          bbECS_entity** return_entity,
                                          char* key,
                                          bbInstruction_source source,
                                          bbHandle action);

bbFlag bbCoreInput_spawnEmptyEntity(bbCore* core,
                                    bbECS* ECS,
                                    char* key,
                                    bbInstruction_source source,
                                    bbHandle action);

bbFlag bbInstruction_spawnEmptyEntity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unspawnEmptyEntity_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbCS_spawnTestEntity(bbCore* core,
                            bbECS_entity** return_entity,
                            bbMapCoords MC,
                            bbHandle server_entity,
                            bbInstruction_source source,
                            bbHandle action);

bbFlag bbCoreInput_spawnTestEntity(bbCore* core,
                                   bbMapCoords MC,
                                   bbHandle server_entity,
                                   bbInstruction_source source,
                                   bbHandle action);

bbFlag bbInstruction_spawnTestEntity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unspawnTestEntity_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbCS_entity_setComponent(bbCore* core,
                                       bbECS* ECS,
                                       bbHandle entity_handle,
                                       bbHandle component,
                                       bbECS_systems system,
                                       bbInstruction_source source,
                                       bbHandle action);

bbFlag bbCoreInput_entity_setComponent(bbCore* core,
                                       bbECS* ECS,
                                       bbHandle entity,
                                       bbHandle component,
                                       bbECS_systems system,
                                       bbInstruction_source source,
                                       bbHandle action);

bbFlag bbInstruction_entity_setComponent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_entity_unsetComponent_fn(bbCore* core, bbInstruction* instruction);
#endif //BB_ECS_H