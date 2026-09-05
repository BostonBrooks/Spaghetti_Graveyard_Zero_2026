#ifndef BB_ECS_H
#define BB_ECS_H

#include "engine/core/bbInstruction.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbList.h"
#include "engine/logic/bbVPool.h"
#include "engine/logic/bbTerminal.h"

typedef enum
{
    bbECS_ECS,
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
    bbHandle entity_handle;
} bbComponent;

typedef struct
{
    bbComponent component;
    char key[KEY_LENGTH];
    bbListElement_Handle list_element_handle;
    bbECS_states state;
    U64 has_component;
    bbHandle components[bbECS_numSystems];
} bbECS_entity;


struct bbSystem;

typedef bbFlag bbHandle_getComponent_fn(struct bbSystem* system, bbComponent** component, bbHandle component_handle);
typedef bbFlag bbComponent_getHandle_fn(struct bbSystem* system, bbComponent* component, bbHandle* component_handle);

typedef struct bbSystem
{
    bbVPool* pool;
    bbHandle_getComponent_fn* getComponent;
    bbComponent_getHandle_fn* getHandle;
} bbSystem;
typedef struct bbECS
{
    bbSystem system;
    bbList list;
    bbHandle player_character;
    bbSystem* systems[];

} bbECS;


bbFlag bbHandle_getComponent(struct bbSystem* system, bbComponent** component, bbHandle component_handle);
bbFlag bbComponent_getHandle(struct bbSystem* system, bbComponent* component, bbHandle* component_handle);

bbFlag bbHandle_mapComponent(bbECS* ECS,
                             bbECS_systems system,
                             bbHandle from_handle,
                             bbECS_systems component_system,
                             bbHandle* component_handle,
                             bbComponent** component);

bbFlag bbComponent_mapComponent(bbECS* ECS,
                             bbECS_systems system,
                             bbComponent* from_component,
                             bbECS_systems component_system,
                             bbHandle* component_handle,
                             bbComponent** component);

bool bbECS_entity_hasComponent(bbECS_entity* entity, bbECS_systems system);

bbFlag bbECS_new(bbECS** ECS, I32 num_systems);

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
                                   bbECS* ECS,
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

///spawn a delete entity instruction
bbFlag bbCoreInput_entity_deleteEntity(bbCore* core,
                                       bbECS* ECS,
                                       bbHandle entity);

///set aside all entity data ///could do this in the calling thread?
bbFlag bbInstruction_entity_deleteEntity_fn(bbCore* core, bbInstruction* instruction);

///restore entity and all data
bbFlag bbInstruction_entity_undeleteEntity_fn(bbCore* core, bbInstruction* instruction);

///discard data needed to restore entity
bbFlag discard_entity_undeleteEntity_fn(bbCore* core, bbInstruction* undo_instruction);
#endif //BB_ECS_H