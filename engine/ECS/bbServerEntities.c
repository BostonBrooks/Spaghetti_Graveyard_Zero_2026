#include "engine/ECS/bbServerEntities.h"

#include <stdlib.h>

#include "engine/core/bbAction.h"
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

    bbDebug("index = %d, collision = %d\n",
        instruction->data.three_handles.handle2.bloated.index,
        instruction->data.three_handles.handle2.bloated.collision);

    //Use server_entity_handle to look up entity_handle
    bbHandle* handle;
    bbVPool_allocFromHandle(home.ECS.server_entities->ECS_Handles, (void*)&handle, component->server_entity_handle );
    *handle = instruction->data.three_handles.handle1;;

    bbCoreInput_entity_setComponent(core,&home.ECS.ECS, entity_handle,component_handle, bbECS_ServerEntities, bbInstructionSource_internal, no_handle);

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unsetServerEntity;
    undo_instruction->data.three_handles.handle1 = component_handle;
    undo_instruction->source = instruction->source;

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
        return bbSuccess;
    }

    return bbSuccess;
}
bbFlag bbInstruction_unsetServerEntity_fn(bbCore* core, bbInstruction* instruction)
{

    bbHandle component_handle = instruction->data.three_handles.handle1;
    bbServerEntity* component;
    bbVPool_lookup(home.ECS.server_entities->system.pool, (void**)&component, component_handle);
    bbHandle server_entity_handle = component->server_entity_handle;
    //bbVPool_free(home.ECS.server_entities->system.pool,component);

    bbDebug("#index = %d, collision = %d\n",
    server_entity_handle.bloated.index,
    server_entity_handle.bloated.collision);

    void* lemon;
    bbVPool_lookup(home.ECS.server_entities->ECS_Handles, (void*)&lemon, server_entity_handle );
    bbVPool_free(home.ECS.server_entities->ECS_Handles, lemon);

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        //TODO place instruction->redo_instruction into core->action_queue
        bbAction* redo_action;
        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }

}