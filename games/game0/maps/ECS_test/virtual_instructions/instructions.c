
#include "instructions.h"

#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/ECS/ECS.h"
#include "engine/ECS/server_entities/bbServerEntities.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbString.h"

bbFlag bbInstruction_spawnServerEntity_fn(bbCore* core, bbInstruction* instruction)
{
    //TODO undo instruciton
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
bbFlag bbInstruction_unspawnServerEntity_fn(bbCore* core, bbInstruction* instruction){bbHere()}

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