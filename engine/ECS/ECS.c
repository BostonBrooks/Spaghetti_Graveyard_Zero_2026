#include "engine/ECS/ECS.h"

#include "engine/core/bbAction.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbString.h"

bbFlag bbECS_init(bbECS* ECS)
{
        bbVPool_newBloated(&ECS->pool, sizeof(bbECS_entity), 1000, 1000, "ECS");
        bbList_init(&ECS->list, ECS->pool, NULL, offsetof(bbECS_entity, list_element_handle),NULL);
        return bbSuccess;
}

bbFlag bbCoreSynchronous_spawnTestEntity(bbCore* core, bbECS* ECS, bbECS_entity** entity, char* key, bbInstruction_source source, bbHandle action)
{
        bbECS_entity* new_entity;
        bbVPool_alloc(ECS->pool, (void**)&new_entity);

        bbHandle new_handle;
        bbVPool_reverseLookup(ECS->pool,new_entity,&new_handle);

        new_entity->state = bbECS_alive;
        new_entity->has_component = 0;
        bbStr_setStr(new_entity->key, key, KEY_LENGTH);
        bbList_pushR(&ECS->list, new_entity);

        *entity = new_entity;

        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbInstruction_unspawnEntity;
        undo_instruction->source = source;
        undo_instruction->data.three_handles.handle1 = new_handle;
        undo_instruction->ECS = ECS;
        //if (source == bbInstructionSource_internal)
        {
                //bbVPool_free(core->instruction_pool, (void*)instruction);
                undo_instruction->redo_instruction.u64 = 0;
                bbList_pushL(&core->undo_stack, (void*)undo_instruction);
                return bbSuccess;
        }
        //if (source == bbInstructionSource_input)
        //{
        //        bbHandle handle;
        //        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        //        undo_instruction->redo_instruction = handle;
        //        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
        //        return bbSuccess;
        //}
        //if (source == bbInstructionSource_action)
        //{
        //        undo_instruction->redo_instruction = instruction->redo_instruction;
        //        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
        //        return bbSuccess;
        //}

        bbNotHere()

}

bbFlag bbCoreInput_spawnTestEntity(bbCore* core, bbECS* ECS, char* key, bbInstruction_source source, bbHandle action)
{
        bbInstruction* instruction;
        bbList_alloc(&core->do_stack, (void**) &instruction);
        instruction->type = bbInstruction_spawnEntity;
        instruction->ECS = ECS;
        bbStr_setStr(instruction->data.key, key, KEY_LENGTH);
        instruction->source = source;
        instruction->redo_instruction = action;
        bbList_pushL(&core->do_stack, instruction);
        return bbSuccess;

}

bbFlag bbInstruction_spawnTestEntity_fn(bbCore* core, bbInstruction* instruction)
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
        undo_instruction->type = bbInstruction_unspawnEntity;
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

bbFlag bbInstruction_unspawnTestEntity_fn(bbCore* core, bbInstruction* instruction)
{


        bbECS* ECS = instruction->ECS;
        bbHandle entity_handle = instruction->data.three_handles.handle1;
        bbECS_entity* entity;

        bbDebug("entity_handle = %d, collision = %d\n", entity_handle.bloated.index, entity_handle.bloated.collision);
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