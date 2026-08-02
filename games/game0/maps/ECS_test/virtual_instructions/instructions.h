#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/ECS/ECS_instructions.h"
#include "engine/logic/bbFlag.h"

typedef enum
{
    bbInstruction_spawnServerEntity = bbInstruction_numECS_Instructions,
    bbInstruction_unspawnServerEntity
} bbMap_instruction_type;



bbFlag bbInstruction_spawnServerEntity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unspawnServerEntity_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbCoreInput_spawnServerEntity(bbCore* core,
                                    char* key,
                                    bbHandle server_entity,
                                    bbInstruction_source source,
                                    bbHandle action);



#endif