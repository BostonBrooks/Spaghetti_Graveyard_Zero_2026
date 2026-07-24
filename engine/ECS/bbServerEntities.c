#include "engine/ECS/bbServerEntities.h"

#include <stdlib.h>

#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbFlag.h"

bbFlag bbServerEntities_new(bbServerEntities** server_entities)
{
    bbServerEntities* system = malloc(sizeof(bbServerEntities));

    bbVPool_newBloated(&system->system.pool, sizeof(bbServerEntity), 100, 100, "SERVER ENTITIES");
    bbVPool_newBloated(&system->ECS_Handles, sizeof(bbHandle), 100, 100, "ENTITIES SERVER");

    *server_entities = system;
    home.ECS.ECS.systems[bbECS_ServerEntities] = (bbSystem* )system;

    return bbSuccess;
}

bbFlag bbCoreInput_setServerEntity(bbCore* core,
                                   bbHandle entity_handle,
                                   bbHandle server_entity_handle,
                                   bbInstruction_source source,
                                   bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbInstruction_setServerEntity;
    instruction->data.three_handles.handle1 = entity_handle;
    instruction->data.three_handles.handle2 = server_entity_handle;
    instruction->ECS = &home.ECS.ECS;
    instruction->source = source;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction);
}

bbFlag bbInstruction_setServerEntity_fn(bbCore* core, bbInstruction* instruction)
{
    //Use component_handle to look up server_entity_handle
    bbHandle component_handle;
    bbServerEntity* component;
    bbVPool_alloc2(home.ECS.server_entities->system.pool, (void**)&component, &component_handle);

    bbHandle entity_handle = instruction->data.three_handles.handle1;
    component->bbECS_entity_handle = entity_handle;
    component->server_entity_handle = instruction->data.three_handles.handle2;


    //Use server_entity_handle to look up entity_handle
    bbHandle* handle;
    bbVPool_allocFromHandle(home.ECS.server_entities->ECS_Handles, (void*)&handle, instruction->data.three_handles.handle2);
    *handle = instruction->data.three_handles.handle1;;

    bbCoreInput_entity_setComponent(core,&home.ECS.ECS, entity_handle,component_handle, bbECS_ServerEntities, bbInstructionSource_internal, no_handle);

    //TODO add undo instruction to undo stack

    return bbSuccess;
}
bbFlag bbInstruction_unsetServerEntity_fn(bbCore* core, bbInstruction* instruction)
{
    bbNotHere()
}