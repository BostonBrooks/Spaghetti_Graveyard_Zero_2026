
#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "../../../../../engine/ECS/entity_spawner/bbEntitySpawner.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"
#include "games/game0/maps/systems_test/entity_spawner/live_spawn.h"

#include "core/instructions.h"
#include "engine/data/bbHome.h"


// typedef struct
// {
//     bbMapCoords position;
//     bbMapCoords goalpoint;
//     bbHandle server_handle;
//     bbHandle goal_server_handle;
// } bbSpawnFunctionArgs;

///create instruction to spawn entity
bbFlag bbCI_live_spawnEntity(bbCore* core, bbSpawnFunctionArgs args, char* key, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    bbHandle spawn_function_handle;
    bbDictionary_lookup(home.ECS.spawner.live_spawn_dict,key,&spawn_function_handle);

    instruction->data.sfArgs.type = spawn_function_handle.u64;
    instruction->data.sfArgs.position = args.position;
    instruction->data.sfArgs.goalpoint = args.goalpoint;
    instruction->data.sfArgs.handle = args.handle;
    instruction->data.sfArgs.goal_handle = args.goal_handle;
    instruction->data.sfArgs.speed = args.speed;
    instruction->data.sfArgs.radius = args.radius;

    instruction->type = bbI_live_spawnEntity;
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;
}

///create undo instruction then spawn entity from function
bbFlag bbI_live_spawnEntity_fn(bbCore* core, bbInstruction* instruction)
{
        bbInstruction* undo_instruction = NULL;
    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_live_unspawnEntity;
        undo_instruction->source = instruction->source;
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_live_unspawnEntity;
        undo_instruction->source = instruction->source;
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (instruction->source == bbInstructionSource_action)
    {
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_live_unspawnEntity;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);


    } //else source == no rewind

    bbHandle undo_handle;
    bbLiveSpawnFunction* function = home.ECS.spawner.live_spawn_functions[instruction->data.sfArgs.type];

    function(&home.ECS.spawner,
             &undo_handle,
             instruction->data.sfArgs,
             bbInstructionSource_internal);

    if (undo_instruction != NULL)
    {
        undo_instruction->data.sfArgs.handle = undo_handle;
    }
    return bbSuccess;
}

///create redo instruction
bbFlag bbI_live_unspawnEntity_fn(bbCore* core, bbInstruction* instruction)
{



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
    bbAssert(0==1, "We should not get here\n");
}