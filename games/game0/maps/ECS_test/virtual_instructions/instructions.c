
#include "instructions.h"

#include "engine/core/bbAction.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/ECS/ECS.h"
#include "engine/ECS/server_entities/bbServerEntities.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbString.h"

bbFlag bbInstruction_spawnServerEntity_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbInstruction_unspawnServerEntity;
        undo_instruction->source = instruction->source;
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);

    } else
        if (instruction->source == bbInstructionSource_input)
        {
            bbInstruction* undo_instruction;
            bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
            undo_instruction->type = bbInstruction_unspawnServerEntity;
            undo_instruction->source = instruction->source;
            bbHandle handle;
            bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
            undo_instruction->redo_instruction = handle;
            bbList_pushL(&core->undo_stack,(void*)undo_instruction);

        } else
            if (instruction->source == bbInstructionSource_action)
            {
                bbInstruction* undo_instruction;
                bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
                undo_instruction->type = bbInstruction_unspawnServerEntity;
                undo_instruction->source = instruction->source;
                undo_instruction->redo_instruction = instruction->redo_instruction;
                bbList_pushL(&core->undo_stack,(void*)undo_instruction);

            } //else source == no rewind
    bbHere()
    bbECS_entity* entity;
    bbCoreSynchronous_spawnEmptyEntity(core,
                                       core->ECS,
                                       &entity,
                                       instruction->data.key,
                                       bbInstructionSource_internal,
                                       no_handle);
    bbHandle handle;
    bbVPool_reverseLookup(core->ECS->system.pool,entity,&handle);

    bbHandle server_handle;
    server_handle.bloated.index = 343;
    server_handle.bloated.collision = 343;

    bbCoreSynchronous_setServerEntity(core,
                                   handle,
                                   server_handle,
                                   bbInstructionSource_internal,
                                   no_handle);



    return bbSuccess;
}
bbFlag bbInstruction_unspawnServerEntity_fn(bbCore* core, bbInstruction* instruction){
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
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
}

bbFlag bbCoreInput_spawnServerEntity(bbCore* core,
                                    char* key,
                                    bbHandle server_entity,
                                    bbInstruction_source source,
                                    bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbInstruction_spawnServerEntity;
    bbStr_setStr(instruction->data.key, key, KEY_LENGTH);
    instruction->source = source;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;

}