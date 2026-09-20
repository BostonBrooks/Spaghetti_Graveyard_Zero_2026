#ifndef ECS_INSTRUCTIONS
#define ECS_INSTRUCTIONS

#include "engine/core/bbInstruction.h"

///Instruction types used by the ECS system
typedef enum {
    bbI_ECS_spawnEmptyEntity = bbInstruction_numTypes,
    bbI_ECS_unspawnEmptyEntity,
    bbI_ECS_entity_setComponent,
    bbI_ECS_entity_unsetComponent,
    bbI_ECS_setServerEntity,
    bbI_ECS_unsetServerEntity,
    bbI_ECS_moveable_setState,
    bbI_ECS_moveable_unsetState,
    bbI_ECS_moveable_setDead,
    bbI_ECS_moveable_unsetDead,
    bbI_ECS_entity_deleteEntity,
    bbI_ECS_entity_undeleteEntity,
    bbI_ECS_spatial_spawn,
    bbI_ECS_spatial_unspawn,
    bbI_ECS_numECS_Instructions,
} bbECS_Instruction_type;

#endif //ECS_INSTRUCTIONS