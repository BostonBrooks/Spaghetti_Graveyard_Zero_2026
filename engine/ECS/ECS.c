#include "engine/ECS/ECS.h"
#include <stddef.h>
#include "engine/core/bbAction.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbString.h"
#include "engine/ECS/ECS_instructions.h"

extern bbHandle null_handle;

U64 getMask(bbECS_systems system)
{
        return 1 << system;
}

bbFlag bbECS_getComponent_fn(bbSystem* system, bbComponent** component, bbHandle component_handle);
bbFlag bbECS_getHandle_fn(bbSystem* system, bbComponent* component, bbHandle* component_handle);


bool bbECS_entity_hasComponent(bbECS_entity* entity, bbECS_systems system)
{
    U64 mask = getMask(system);

    if (mask & entity->has_component) return true;
    else return false;
}




bbFlag bbECS_init(bbECS* ECS, I32 num_systems)
{
    ECS->systems = calloc(num_systems, sizeof(bbSystem));
    ECS->systems[bbECS_ECS] = (bbSystem*)ECS;
    bbVPool_newBloated(&ECS->system.pool, sizeof(bbECS_entity), 100, 100, "ECS");
    bbList_init(&ECS->list, ECS->system.pool, NULL, offsetof(bbECS_entity, list_element_handle),NULL);

    ECS->system.getComponent = bbECS_getComponent_fn;
    ECS->system.getHandle = bbECS_getHandle_fn;

    return bbSuccess;
}

bbFlag bbCoreSynchronous_spawnEmptyEntity(bbCore* core, bbECS* ECS, bbECS_entity** return_entity, char* key, bbInstruction_source source, bbHandle action)
{
    bbECS_entity* new_entity;
    bbHandle new_handle;
    bbVPool_alloc2(ECS->system.pool, (void**)&new_entity,&new_handle);
    new_entity->state = bbECS_alive;
    new_entity->has_component = 0;
    new_entity->component.entity_handle = new_handle;
    bbStr_setStr(new_entity->key, key, KEY_LENGTH);
    bbList_pushR(&ECS->list, new_entity);

//Set entity.ecs_entity = entity
    U64 mask = getMask(bbECS_ECS);
    new_entity->has_component |= mask;
    new_entity->components[bbECS_ECS] = new_handle;

    bbDebug("entity.key = %s\n", new_entity->key);

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        bbInstruction* instruction;
        bbHandle instruction_handle;
        bbFlag flag = bbList_alloc2(&core->do_stack,(void**)&instruction, &instruction_handle);

        //set input instruction data
        instruction->type = bbInstruction_spawnEmptyEntity;
        instruction->ECS = ECS;
        bbStr_setStr(instruction->data.key, key, KEY_LENGTH);
        instruction->source = source;
        instruction->redo_instruction = action;

        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction_handle;

        //set instruction data
        undo_instruction->type = bbInstruction_unspawnEmptyEntity;
        undo_instruction->source = source;
        undo_instruction->data.three_handles.handle1 = new_handle;
        undo_instruction->ECS = ECS;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = source;

        //set instruction data
        undo_instruction->type = bbInstruction_unspawnEmptyEntity;
        undo_instruction->source = source;
        undo_instruction->data.three_handles.handle1 = new_handle;
        undo_instruction->ECS = ECS;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        undo_instruction->type = bbInstruction_unspawnEmptyEntity;
        undo_instruction->source = source;
        undo_instruction->data.three_handles.handle1 = new_handle;
        undo_instruction->ECS = ECS;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_norewind)
    {

    }


    *return_entity = new_entity;

    return bbSuccess;
}

bbFlag bbCoreInput_spawnEmptyEntity(bbCore* core, bbECS* ECS, char* key, bbInstruction_source source, bbHandle action)
{
        bbInstruction* instruction;
        bbList_alloc(&core->do_stack, (void**) &instruction);
        instruction->type = bbInstruction_spawnEmptyEntity;
        instruction->ECS = ECS;
        bbStr_setStr(instruction->data.key, key, KEY_LENGTH);
        instruction->source = source;
        instruction->redo_instruction = action;
        bbList_pushL(&core->do_stack, instruction);
        return bbSuccess;

}

bbFlag bbInstruction_spawnEmptyEntity_fn(bbCore* core, bbInstruction* instruction)
{
        bbECS* ECS = instruction->ECS;
        bbECS_entity* new_entity;
        bbHandle new_handle;
        bbVPool_alloc2(ECS->system.pool, (void**)&new_entity,&new_handle);


        new_entity->state = bbECS_alive;
        new_entity->has_component = 0;
        bbStr_setStr(new_entity->key, instruction->data.key, KEY_LENGTH);
        bbList_pushR(&ECS->list, new_entity);

    //Set entity.ecs_entity = entity
    U64 mask = getMask(bbECS_ECS);
    new_entity->has_component |= mask;
    new_entity->components[bbECS_ECS] = new_handle;

    bbDebug("entity.key = %s\n", new_entity->key);

        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbInstruction_unspawnEmptyEntity;
        undo_instruction->source = instruction->source;
        undo_instruction->data.three_handles.handle1 = new_handle;
        undo_instruction->ECS = ECS;

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

        bbNotHere()
    }

bbFlag bbInstruction_unspawnEmptyEntity_fn(bbCore* core, bbInstruction* instruction)
{bbHere()
    bbECS* ECS = instruction->ECS;
    bbHandle entity_handle = instruction->data.three_handles.handle1;
    bbECS_entity* entity;

    bbVPool_lookup(ECS->system.pool, (void**)&entity, entity_handle);
    entity->state = bbECS_unused;
    bbList_remove(&ECS->list, entity);
    bbVPool_free(ECS->system.pool, (void*)entity);

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction,
                       instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action,
                       instruction->redo_instruction);
        bbList_sortL(&core->action_queue, (void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }

    bbNotHere()
}

bbFlag bbCS_entity_setComponent(bbCore* core,
                                bbECS* ECS,
                                bbHandle entity_handle,
                                bbHandle component,
                                bbECS_systems system,
                                bbInstruction_source source,
                                bbHandle action)
{bbHere()
    //Post undo instruction

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        bbInstruction* instruction;
        bbHandle instruction_handle;
        bbFlag flag = bbList_alloc2(&core->do_stack, (void**)&instruction,&instruction_handle);

        instruction->source = source;

        //set input instruction data
        instruction->type = bbInstruction_entity_setComponent;
        instruction->ECS = ECS;
        instruction->data.three_handles.handle1 = entity_handle;
        instruction->data.three_handles.handle2 = component;
        instruction->data.three_handles.handle3.u64 = system;
        //bbStr_setStr(instruction->data.key, string, KEY_LENGTH);

        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction_handle;

        //set instruction data
        undo_instruction->type = bbInstruction_entity_unsetComponent;
        undo_instruction->data.three_handles.handle1 = entity_handle;
        undo_instruction->data.three_handles.handle2 = component;
        undo_instruction->data.three_handles.handle3.u64 = system;
        undo_instruction->ECS = ECS;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = source;

        //set instruction data
        undo_instruction->type = bbInstruction_entity_unsetComponent;
        undo_instruction->data.three_handles.handle1 = entity_handle;
        undo_instruction->data.three_handles.handle2 = component;
        undo_instruction->data.three_handles.handle3.u64 = system;
        undo_instruction->ECS = ECS;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        undo_instruction->type = bbInstruction_entity_unsetComponent;
        undo_instruction->data.three_handles.handle1 = entity_handle;
        undo_instruction->data.three_handles.handle2 = component;
        undo_instruction->data.three_handles.handle3.u64 = system;
        undo_instruction->ECS = ECS;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (source == bbInstructionSource_norewind)
    {
    }

    //take action / post side-effects
    //
    U64 mask = getMask(system);
    bbECS_entity* entity;
    bbVPool_lookup(ECS->system.pool, (void**)&entity, entity_handle);

    if (mask & entity->has_component)
    {
        return bbFail;
    }

    bbAssert(!bbECS_entity_hasComponent(entity, system), "Entity already has component\n");
    entity->has_component |= mask;
    entity->components[system] = component;

    return bbSuccess;
}

bbFlag bbCoreInput_entity_setComponent(bbCore* core,bbECS* ECS, bbHandle entity,
    bbHandle component, bbECS_systems system, bbInstruction_source source, bbHandle action)
{
    {
        bbInstruction* instruction;
        bbList_alloc(&core->do_stack, (void**) &instruction);
        instruction->type = bbInstruction_entity_setComponent;
        instruction->ECS = ECS;
        instruction->source = source;
        instruction->redo_instruction = action;
        instruction->data.three_handles.handle1 = entity;
        instruction->data.three_handles.handle2 = component;
        instruction->data.three_handles.handle3.u64 = system;
        bbList_pushL(&core->do_stack, instruction);
        return bbSuccess;

    }
}

bbFlag bbInstruction_entity_setComponent_fn(bbCore* core, bbInstruction* instruction)
{bbHere()
    bbECS* ECS = instruction->ECS;
    bbHandle entity_handle = instruction->data.three_handles.handle1;
    bbHandle component = instruction->data.three_handles.handle2;
    bbECS_systems system = instruction->data.three_handles.handle3.u64;


    U64 mask = getMask(system);
    bbECS_entity* entity;
    bbVPool_lookup(ECS->system.pool, (void**)&entity, entity_handle);


    bbAssert(!bbECS_entity_hasComponent(entity, system), "Entity already has component\n");

    if (mask & entity->has_component)
    {
        return bbFail;
    }
    entity->has_component |= mask;
    entity->components[system] = component;

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_entity_unsetComponent;
    undo_instruction->source = instruction->source;

    undo_instruction->data.three_handles.handle1 = entity_handle;
    undo_instruction->data.three_handles.handle2 = component;
    undo_instruction->data.three_handles.handle3.u64 = system;
    undo_instruction->ECS = ECS;

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

}

bbFlag bbInstruction_entity_unsetComponent_fn(bbCore* core, bbInstruction* instruction)
{bbHere()
    bbECS* ECS = instruction->ECS;
    bbHandle entity_handle = instruction->data.three_handles.handle1;
    bbHandle component = instruction->data.three_handles.handle2;
    bbECS_systems system = instruction->data.three_handles.handle3.u64;

    bbECS_entity* entity;
    bbVPool_lookup(ECS->system.pool, (void**)&entity, entity_handle);
    U64 mask = getMask(system);


    bbAssert(bbECS_entity_hasComponent(entity, system), "Entity doesnt already have component\n");

    entity->has_component &= ~mask;

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction,
                       instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action,
                       instruction->redo_instruction);
        bbList_sortL(&core->action_queue, (void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    bbHere()
}

/*
bbFlag bbCoreInput_spawnTestEntity(bbCore* core, bbECS* ECS, bbMapCoords MC, bbHandle server_entity, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbInstruction_spawnTestEntity;
    instruction->data.agent_MC.coords = MC;
    instruction->data.agent_MC.handle1 = server_entity;
    instruction->ECS = ECS;
    instruction->source = source;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}*/
/*
bbFlag bbInstruction_spawnTestEntity_fn(bbCore* core, bbInstruction* instruction)
{
    bbECS_entity* entity;
    bbCoreSynchronous_spawnEmptyEntity(core, instruction->ECS, &entity, instruction->data.key, bbInstructionSource_internal, no_handle);

    bbHandle server_entity = instruction->data.agent_MC.handle1;
    bbMapCoords MC = instruction->data.agent_MC.coords;
    bbHandle entity_handle, moveable_handle;
    bbECS* ECS = instruction->ECS;
    bbVPool_reverseLookup(ECS->system.pool, (void*)entity, &entity_handle);


    //TODO This should be a core input?

    bbCoreSynchronous_spawnTestMoveable(core,
                                        entity_handle,
                                        &moveable_handle,
                                        MC,
                                        bbInstructionSource_internal,
                                        no_handle);

    bbCoreInput_entity_setComponent(core,ECS, entity_handle,moveable_handle, bbECS_Moveables, bbInstructionSource_internal, no_handle);
    bbCoreInput_entity_setComponent(core,ECS, entity_handle,no_handle, bbECS_AI, bbInstructionSource_internal, no_handle);
    bbCoreInput_entity_setComponent(core,ECS, entity_handle,no_handle, bbECS_Graphics, bbInstructionSource_internal, no_handle);

    bbHandle test_handle = entity_handle;

    if (server_entity.u64 != no_handle.u64)
    {
        bbCoreInput_setServerEntity(core,
                                    entity_handle,
                                    server_entity,
                                    bbInstructionSource_internal,
                                    no_handle);
    }

    if (home.UI.inbox.local_message_pool!=0)
    {
        bbCoreInput_spawnGraphicsComponent(core, MC,entity_handle, moveable_handle, bbInstructionSource_internal, no_handle);
    }

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unspawnTestEntity;
    undo_instruction->source = instruction->source;
    undo_instruction->data.three_handles.handle1 = entity_handle;
    undo_instruction->ECS = ECS;

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
*/

/*
bbFlag bbInstruction_unspawnTestEntity_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction,
                       instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action,
                       instruction->redo_instruction);
        bbList_sortL(&core->action_queue, (void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
}
*/
bbFlag bbECS_getComponent_fn(bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    return bbVPool_lookup(system->pool,(void**)component,component_handle);
}

bbFlag bbECS_getHandle_fn(bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    return bbVPool_reverseLookup(system->pool,(void*)component,component_handle);
}

bbFlag bbHandle_getComponent(bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    bbHandle_getComponent_fn* function = system->getComponent;
    return function(system, component, component_handle);
}
bbFlag bbComponent_getHandle(bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    bbComponent_getHandle_fn* function = system->getHandle;
    return function(system, component, component_handle);
}

bbFlag bbComponent_mapComponent(bbECS* ECS,
                             bbECS_systems system,
                             bbComponent* from_component,
                             bbECS_systems component_system,
                             bbHandle* component_handle,
                             bbComponent** component)
{
    bbHandle entity_handle = from_component->entity_handle;

    bbECS_entity* entity;

    bbVPool_lookup(ECS->system.pool, (void**)&entity, entity_handle);

    bbHandle_getComponent(&ECS->system,(bbComponent**)&entity,entity_handle);

    bbSystem* system1 = ECS->systems[component_system];

    bbHandle component_handle1 = entity->components[component_system];
    bbComponent* component1;
    bbHandle_getComponent(system1,&component1,component_handle1);

    bbAssert(component1 != NULL,"Something returned null\n");

    if (component_handle!=NULL) *component_handle = component_handle1;
    if (component!=NULL)*component = component1;




    return bbSuccess;
}

bbFlag bbHandle_mapComponent(bbECS* ECS,
                             bbECS_systems system,
                             bbHandle from_handle,
                             bbECS_systems component_system,
                             bbHandle* component_handle,
                             bbComponent** component)
{
    bbComponent* component1;
    bbSystem* system1 = ECS->systems[system];
    bbHandle_getComponent(system1,&component1,from_handle);

    return bbComponent_mapComponent(ECS,
                             system,
                             component1,
                             component_system,
                             component_handle,
                             component);
}