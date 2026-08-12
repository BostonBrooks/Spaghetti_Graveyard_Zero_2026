#ifndef ECS_INSTRUCTIONS
#define ECS_INSTRUCTIONS

#include "engine/core/bbInstruction.h"

typedef enum {
    bbInstruction_spawnEmptyEntity = bbInstruction_numTypes,
    bbInstruction_unspawnEmptyEntity,
    bbInstruction_entity_setComponent,
    bbInstruction_entity_unsetComponent,
    bbInstruction_setServerEntity,
    bbInstruction_unsetServerEntity,
    bbI_moveable_setState,
    bbI_moveable_unsetState,
    bbInstruction_numECS_Instructions
} bbECS_Instruction_type;

#endif //ECS_INSTRUCTIONS