/** The Entity Component System tracks "things" in the game world, such as monsters,
 * Player characters, etc
 * We need to be able to spawn an empty entity then add a bunch of components.
 * This instruction will need to be undo-able in the core
 *
 * Deleting an entity if entity.state == dying removes the entity from ECS.list
 * This action is undoable in the core.
 * if the undo instruction is discarded, the entity is freed in the pool
 */

#ifndef BB_ECS_H_OLD
#define BB_ECS_H_OLD

#include "engine/core/bbInstruction.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbList.h"
#include "engine/logic/bbVPool.h"

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

/*
bbFlag bbECS_update(bbECS* ECS);
bbFlag bbECS_newEntity(bbECS* ECS, bbHandle* handle, bbECS_entity** entity);
bbFlag bbECS_entity_getComponent(bbECS* ECS, bbHandle* component, bbHandle entity, bbECS_systems system);
bbFlag bbECS_entity_setComponent(bbECS* ECS, bbHandle component, bbHandle entity, bbECS_systems system);


///Spawn entity immediately then add undo/redo instructions in the core. set bbHandle* entity to point to the spawned entity
bbFlag bbCoreImmediate_spawnEntity(bbCore* core, bbECS* ECS, bbHandle* entity, bbInstruction_source source, bbHandle action);

///pretty useless since you don't get a handle to the new entity
bbFlag bbCoreInput_spawnEntity(bbCore* core, bbInstruction_source source, bbHandle action);
bbFlag bbInstruction_spawnEntity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unspawnEntity_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbCoreInput_deleteEntity_fn(bbCore* core,bbInstruction_source source, bbHandle entity);
bbFlag bbInstruction_deleteEntity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_undeleteEntity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_confirmDeleteEntity_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbCoreInput_entity_setComponent(bbCore* core,bbECS* ECS, bbHandle entity,
    bbHandle component, bbECS_systems system, bbInstruction_source source, bbHandle action);
bbFlag bbInstruction_entity_setComponent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_entity_unsetComponent_fn(bbCore* core, bbInstruction* instruction);
*/
#endif //BB_ECS_H_OLD