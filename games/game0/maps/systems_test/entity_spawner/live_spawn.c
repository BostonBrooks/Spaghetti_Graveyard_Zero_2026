
#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "../../../../../engine/ECS/entity_spawner/bbEntitySpawner.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"
#include "games/game0/maps/systems_test/entity_spawner/live_spawn.h"

#include "core/instructions.h"
#include "engine/core/bbInstruction_operations.h"
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

    allocActiveInstruction(instruction)

    bbHandle spawn_function_handle;
    bbDictionary_lookup(home.ECS.spawner.live_spawn_dict,key,&spawn_function_handle);

    instruction->data.sfArgs.type = spawn_function_handle.u64;
    instruction->data.sfArgs.position = args.position;
    instruction->data.sfArgs.goalpoint = args.goalpoint;
    instruction->data.sfArgs.handle = args.handle;
    instruction->data.sfArgs.goal_handle = args.goal_handle;
    instruction->data.sfArgs.speed = args.speed;
    instruction->data.sfArgs.radius = args.radius;
    instruction->data.sfArgs.mass = args.mass;


    instruction->type = bbI_live_spawnEntity;
    instruction->source = source;
    instruction->redo_instruction = action;

    pushActiveInstruction(instruction)

    return bbSuccess;
}

///create undo instruction then spawn entity from function
bbFlag bbI_live_spawnEntity_fn(bbCore* core, bbInstruction* instruction)
{
    bbHandle undo_handle;
    bbLiveSpawnFunction* function = home.ECS.spawner.live_spawn_functions[instruction->data.sfArgs.type];

    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_live_unspawnEntity;
        undo_instruction->source = instruction->source;
        //bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        undo_instruction->data.sfArgs.handle = undo_handle;
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_live_unspawnEntity;
        undo_instruction->source = instruction->source;
        undo_instruction->data.sfArgs.handle = undo_handle;
        allocRedoInstruction(redo_instruction)
         *redo_instruction = *instruction;
        undo_instruction->redo_instruction = (bbHandle)redo_instruction_handle;
        pushRedoInstruction(redo_instruction)
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_action)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_live_unspawnEntity;
        undo_instruction->source = instruction->source;
        undo_instruction->data.sfArgs.handle = undo_handle;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)


    } //else source == no rewind


    function(&home.ECS.spawner,
             &undo_handle,
             &instruction->data.sfArgs,
             bbInstructionSource_internal);


    return bbSuccess;
}

///create redo instruction
bbFlag bbI_live_unspawnEntity_fn(bbCore* core, bbInstruction* instruction)
{



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