#include "engine/ECS(old)/ECS(old).h"

#include "engine/core/bbAction.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"

U64 getMask(bbECS_systems system)
{
    return 1 << system;
}

bbFlag bbECS_init(bbECS* ECS)
{
    bbVPool_newBloated(&ECS->pool, sizeof(bbECS_entity), 1000, 1000, "ECS");
    bbList_init(&ECS->list, ECS->pool, NULL, offsetof(bbECS_entity, list_element_handle),NULL);
    return bbSuccess;
}
/* old code might reuse
bbFlag bbECS_newEntity(bbECS* ECS, bbHandle* handle, bbECS_entity** entity)
{
    bbECS_entity* new_entity;
    bbVPool_alloc(ECS->pool, (void**)&new_entity);


    bbHandle new_handle;
    bbVPool_reverseLookup(ECS->pool,new_entity,&new_handle);

    new_entity->state = bbECS_alive;
    new_entity->has_component = 0;


    bbCoreInput_notifyEntitySpawned(&home.core.core, bbInstructionSource_internal, new_handle);

    bbList_pushR(&ECS->list, new_entity);
    if (handle!=NULL) *handle = new_handle;
    if (entity!=NULL) *entity = new_entity;



    return bbSuccess;
}

bbFlag bbECS_entity_getComponent(bbECS* ECS, bbHandle* component_handle, bbHandle entity_handle, bbECS_systems system)
{
    U64 mask = getMask(system);
    bbECS_entity* entity;
    bbVPool_lookup(ECS->pool, (void**)&entity, entity_handle);

    if (mask && entity->has_component == 0)
    {
        *component_handle = ECS->pool->null;
        return bbFail;
    }

    *component_handle = entity->components[system];
    return bbSuccess;
}

bbFlag bbECS_entity_setComponent(bbECS* ECS, bbHandle component, bbHandle entity_handle, bbECS_systems system)
{
    U64 mask = getMask(system);
    bbECS_entity* entity;
    bbVPool_lookup(ECS->pool, (void**)&entity, entity_handle);

    if (mask && entity->has_component)
    {
        return bbFail;
    }
    entity->has_component &= mask;

    entity->components[system] = component;

    return bbSuccess;
}

bbFlag bbCoreInput_notifyEntitySpawned(bbCore* core, bbHandle entity, bbInstruction_source source, bbHandle action)
{
    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unspawnEntity;
    undo_instruction->source = source;
    undo_instruction->data.three_handles.handle1 = entity;

    if (source == bbInstructionSource_internal)
    {
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
        return bbSuccess;
    }

    if (source == bbInstructionSource_input)
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

    return bbNone;
}
*/

bbFlag bbCoreInput_spawnEntity(bbCore* core, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbInstruction_spawnEntity;
    instruction->source = source;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;

}
//TODO this a cludge
extern bbCore core;
extern bbECS ECS;

bbFlag bbInstruction_spawnEntity_fn(bbCore* core, bbInstruction* instruction)
{
    bbECS_entity* new_entity;
    bbVPool_alloc(ECS.pool, (void**)&new_entity);

    bbHandle new_handle;
    bbVPool_reverseLookup(ECS.pool,new_entity,&new_handle);

    new_entity->state = bbECS_alive;
    new_entity->has_component = 0;

    bbList_pushR(&ECS.list, new_entity);

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unspawnEntity;
    undo_instruction->source = instruction->source;
    undo_instruction->data.three_handles.handle1 = new_handle;

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

bbFlag bbInstruction_unspawnEntity_fn(bbCore* core, bbInstruction* instruction)
{


    bbHandle entity_handle = instruction->data.three_handles.handle1;
    bbECS_entity* entity;
    bbVPool_lookup(ECS.pool, (void**)&entity, entity_handle);
    entity->state = bbECS_unused;
    bbList_remove(&ECS.list, entity);

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
/*
bbFlag bbCoreImmediate_spawnEntity(bbCore* core, bbECS* ECS, bbHandle* entity, bbInstruction_source source, bbHandle action)
{
    bbECS_entity* new_entity;
    bbVPool_alloc(ECS->pool, (void**)&new_entity);

    bbHandle new_handle;
    bbVPool_reverseLookup(ECS->pool,new_entity,&new_handle);

    new_entity->state = bbECS_alive;
    new_entity->has_component = 0;

    bbList_pushR(&ECS->list, new_entity);

    *entity = new_handle;

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unspawnEntity;
    undo_instruction->source = source;
    undo_instruction->data.three_handles.handle1 = new_handle;

    if (source == bbInstructionSource_internal)
    {
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
        return bbSuccess;
    }

    bbNotHere()
}
*/
bbFlag bbCoreInput_entity_setComponent(bbCore* core,bbECS* ECS, bbHandle entity,
    bbHandle component, bbECS_systems system, bbInstruction_source source, bbHandle action)
{
    {
        bbInstruction* instruction;
        bbList_alloc(&core->do_stack, (void**) &instruction);
        instruction->type = bbInstruction_entity_setComponent;
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
{
    bbHandle entity_handle = instruction->data.three_handles.handle1;
    bbHandle component = instruction->data.three_handles.handle2;
    bbECS_systems system = instruction->data.three_handles.handle3.u64;


    U64 mask = getMask(system);
    bbECS_entity* entity;
    bbVPool_lookup(ECS.pool, (void**)&entity, entity_handle);

    if (mask && entity->has_component)
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
{
    bbHandle entity_handle = instruction->data.three_handles.handle1;
    bbHandle component = instruction->data.three_handles.handle2;
    bbECS_systems system = instruction->data.three_handles.handle3.u64;

    bbECS_entity* entity;
    bbVPool_lookup(ECS.pool, (void**)&entity, entity_handle);
    U64 mask = getMask(system);

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