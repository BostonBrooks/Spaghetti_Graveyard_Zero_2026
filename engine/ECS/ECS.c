#include "engine/ECS/ECS.h"
#include <stddef.h>
#include "engine/core/bbAction.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbString.h"


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

bbFlag bbCoreSynchronous_spawnEmptyEntity(bbCore* core, bbECS* ECS, bbECS_entity** return_entity, char* key, bbInstruction_source source, bbHandle action)
{
        bbECS_entity* new_entity;
        bbVPool_alloc(ECS->pool, (void**)&new_entity);

        bbHandle new_handle;
        bbVPool_reverseLookup(ECS->pool,new_entity,&new_handle);

        new_entity->state = bbECS_alive;
        new_entity->has_component = 0;
        bbStr_setStr(new_entity->key, key, KEY_LENGTH);
        bbList_pushR(&ECS->list, new_entity);

        *return_entity = new_entity;

        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbInstruction_unspawnEmptyEntity;
        undo_instruction->source = source;
        undo_instruction->data.three_handles.handle1 = new_handle;
        undo_instruction->ECS = ECS;
        if (source == bbInstructionSource_internal)
        {
                //bbVPool_free(core->instruction_pool, (void*)instruction);
                undo_instruction->redo_instruction.u64 = 0;
                bbList_pushL(&core->undo_stack, (void*)undo_instruction);
                return bbSuccess;
        }
        //if (source == bbInstructionSource_input)?
        //if (source == bbInstructionSource_action)?


        bbNotHere()

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
        bbECS_entity* new_entity;
        bbECS* ECS = instruction->ECS;
        bbVPool_alloc(ECS->pool, (void**)&new_entity);

        bbHandle new_handle;
        bbVPool_reverseLookup(ECS->pool,new_entity,&new_handle);

        new_entity->state = bbECS_alive;
        new_entity->has_component = 0;
        bbStr_setStr(new_entity->key, instruction->data.key, KEY_LENGTH);
        bbList_pushR(&ECS->list, new_entity);

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
{


        bbECS* ECS = instruction->ECS;
        bbHandle entity_handle = instruction->data.three_handles.handle1;
        bbECS_entity* entity;

        bbVPool_lookup(ECS->pool, (void**)&entity, entity_handle);
        entity->state = bbECS_unused;
        bbList_remove(&ECS->list, entity);
        bbVPool_free(ECS->pool, (void*)entity);

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
{
    bbECS* ECS = instruction->ECS;
    bbHandle entity_handle = instruction->data.three_handles.handle1;
    bbHandle component = instruction->data.three_handles.handle2;
    bbECS_systems system = instruction->data.three_handles.handle3.u64;


    U64 mask = getMask(system);
    bbECS_entity* entity;
    bbVPool_lookup(ECS->pool, (void**)&entity, entity_handle);

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
{
    bbECS* ECS = instruction->ECS;
    bbHandle entity_handle = instruction->data.three_handles.handle1;
    bbHandle component = instruction->data.three_handles.handle2;
    bbECS_systems system = instruction->data.three_handles.handle3.u64;

    bbECS_entity* entity;
    bbVPool_lookup(ECS->pool, (void**)&entity, entity_handle);
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

bbFlag bbCoreInput_spawnTestEntity(bbCore* core, bbMapCoords MC, bbHandle server_entity, bbInstruction_source source, bbHandle action)
{bbHere()
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbInstruction_spawnTestEntity;
    instruction->data.agent_MC.coords = MC;
    instruction->data.agent_MC.agent = server_entity;
    instruction->ECS = &home.ECS.ECS;
    instruction->source = source;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}
bbFlag bbInstruction_spawnTestEntity_fn(bbCore* core, bbInstruction* instruction)
{bbHere()
    bbECS_entity* entity;
    bbCoreSynchronous_spawnEmptyEntity(core, instruction->ECS, &entity, instruction->data.key, bbInstructionSource_internal, no_handle);

    bbHandle server_entity = instruction->data.agent_MC.agent;
    bbMapCoords MC = instruction->data.agent_MC.coords;
    bbHandle entity_handle;
    bbECS* ECS = &home.ECS.ECS;
    bbVPool_reverseLookup(ECS->pool, (void*)entity, &entity_handle);
    bbCoreInput_entity_setComponent(core,ECS, entity_handle,no_handle, bbECS_Moveables, bbInstructionSource_internal, no_handle);
    bbCoreInput_entity_setComponent(core,ECS, entity_handle,no_handle, bbECS_AI, bbInstructionSource_internal, no_handle);
    bbCoreInput_entity_setComponent(core,ECS, entity_handle,no_handle, bbECS_Graphics, bbInstructionSource_internal, no_handle);

    bbHandle test_handle = entity_handle;
    bbCoreInput_setServerEntity(core,
                                entity_handle,
                                server_entity,
                                bbInstructionSource_internal,
                                no_handle);

    bbUI_Inbox_NewBanana(&home.UI.inbox, MC, 0, 0);


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