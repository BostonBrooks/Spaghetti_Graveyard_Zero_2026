
#include "instructions.h"

#include "engine/core/bbAction.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/core/bbInstruction_operations.h"
#include "engine/ECS/bbECS.h"
#include "engine/ECS/server_entities/bbServerEntities.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbString.h"

bbFlag bbInstruction_spawnServerEntity_fn(bbCore* core, bbInstruction* instruction)
{bbHere()
    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbInstruction_unspawnServerEntity;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbInstruction_unspawnServerEntity;
        undo_instruction->source = instruction->source;
        allocRedoInstruction(redo_instruction)
         *redo_instruction = *instruction;
        undo_instruction->redo_instruction = redo_instruction_handle;
        pushRedoInstruction(redo_instruction)
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_action)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbInstruction_unspawnServerEntity;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)

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
        //bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        popRedoInstruction(redo_instruction,instruction)
        allocActiveInstruction(new_instruction)
        *new_instruction = redo_instruction;
        bbCore_checkMap(&core->map, &redo_instruction, instruction);

        pushActiveInstruction(new_instruction)
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        //bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    bbAssert(0==1, "We should not get here\n");
}

bbFlag bbCoreInput_spawnServerEntity(bbCore* core,
                                    char* key,
                                    bbHandle server_entity,
                                    bbInstruction_source source,
                                    bbHandle action)
{
    allocActiveInstruction(instruction)
    instruction->type = bbInstruction_spawnServerEntity;
    bbStr_setStr(instruction->data.key, key, KEY_LENGTH);
    instruction->source = source;
    instruction->redo_instruction = action;
    pushActiveInstruction(instruction)
    return bbSuccess;

}