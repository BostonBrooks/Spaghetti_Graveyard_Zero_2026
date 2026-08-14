
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

    instruction->type = bbI_live_spawnEntity;
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;
}

///create undo instruction then spawn entity from function
bbFlag bbI_live_spawnEntity_fn(bbCore* core, bbInstruction* instruction)
{
    bbLiveSpawnFunction* function = home.ECS.spawner.live_spawn_functions[instruction->data.sfArgs.type];

    function(&home.ECS.spawner,
             instruction->data.sfArgs,
             bbInstructionSource_norewind);

    return bbSuccess;
}

///create redo instruction
bbFlag bbI_live_unspawnEntity_fn(bbCore* core, bbInstruction* instruction)
{
    bbNotHere()
}