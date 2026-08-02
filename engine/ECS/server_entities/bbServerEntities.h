/** Server Entity
 * A server entity is an entity that is spawned by a message from the server.
 * The entity is owned by the server and has a handle that is synchronised across
 * every simulation.
 *
 * The server entity system contains server entity components. If an entity is
 * a server entity then it has a server entity component. The server entity system
 * is responsible for mapping server entity handles to and from ECS handles/addresses.
 *
 **/

#ifndef SERVER_ENTITY_H
#define SERVER_ENTITY_H
#include "engine/ECS/ECS.h"

//Component
typedef struct
{
    //always the first element of component
    bbComponent component;
    bbHandle server_entity_handle;
} bbServerEntity;

//System
typedef struct
{
    //Maps from bbECS_entity to server_entity_handle
    bbSystem system;
    //Maps from server_entity_handle to bbECS_entity
    bbVPool* ECS_Handles;
} bbServerEntities;

bbFlag bbServerEntities_init(bbServerEntities* server_entities, bbECS* ECS);

bbFlag bbECS_newServerEntities(bbECS* ECS, bbECS_systems system);

bbFlag bbCoreInput_setServerEntity(bbCore* core,
                                   bbHandle entity,
                                   bbHandle server_entity_handle,
                                   bbInstruction_source source,
                                   bbHandle action);

bbFlag bbCoreSynchronous_setServerEntity(bbCore* core,
                                   bbHandle entity,
                                   bbHandle server_entity_handle,
                                   bbInstruction_source source,
                                   bbHandle action);

bbFlag bbInstruction_setServerEntity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unsetServerEntity_fn(bbCore* core, bbInstruction* instruction);
#endif