

#include <stdlib.h>

#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/logic/bbFlag.h"
#include "instructions.h"
#include "engine/ECS/server_entities/bbServerEntities.h"

bbFlag bbCore_initVInstructions(bbCore* core)
{
    I32 max_instructions = 32;//Careful no tomake this too small;
    core->instruction_functions = calloc(sizeof(bbInstruction_fn*), max_instructions);
    core->instruction_functions[bbInstruction_spawnEmptyEntity-bbInstruction_numTypes] = bbInstruction_spawnEmptyEntity_fn;
    core->instruction_functions[bbInstruction_unspawnEmptyEntity-bbInstruction_numTypes] = bbInstruction_unspawnEmptyEntity_fn;
    core->instruction_functions[bbInstruction_entity_setComponent-bbInstruction_numTypes] = bbInstruction_entity_setComponent_fn;
    core->instruction_functions[bbInstruction_entity_unsetComponent-bbInstruction_numTypes] = bbInstruction_entity_unsetComponent_fn;
    core->instruction_functions[bbInstruction_setServerEntity-bbInstruction_numTypes] = bbInstruction_setServerEntity_fn;
    core->instruction_functions[bbInstruction_unsetServerEntity-bbInstruction_numTypes] = bbInstruction_unsetServerEntity_fn;

    core->instruction_functions[bbInstruction_spawnServerEntity-bbInstruction_numTypes] = bbInstruction_spawnServerEntity_fn;
    core->instruction_functions[bbInstruction_unspawnServerEntity-bbInstruction_numTypes] = bbInstruction_unspawnServerEntity_fn;
    core->instruction_functions[bbInstruction_netpauseButton-bbInstruction_numTypes] = bbInstruction_netpauseButton_fn;
    core->instruction_functions[bbInstruction_unfreezeButton-bbInstruction_numTypes] = bbInstruction_unfreezeButton_fn;
       return bbNone;
}
//     core->instruction_functions
//         = calloc(bbVInstruction_numTypes-bbInstruction_numTypes,
//             sizeof (bbInstruction_fn*));
//
//     core->instruction_functions[bbVInstruction_setGoalpointOut-bbInstruction_numTypes]
//         = bbVInstruction_setGoalpointOut_fn;
//
//     core->instruction_functions[bbVInstruction_setGoalpointIn-bbInstruction_numTypes]
//         = bbVInstruction_setGoalpointIn_fn;
//
//     core->instruction_functions[bbVInstruction_unsetGoalpoint-bbInstruction_numTypes]
//         = bbVInstruction_unsetGoalpoint_fn;
//
//     core->instruction_functions[bbVInstruction_updateMovables-bbInstruction_numTypes]
//         = bbVInstruction_updateMovables_fn;
//
//     core->instruction_functions[bbVInstruction_unupdateMovables-bbInstruction_numTypes]
//         = bbVInstruction_unupdateMovables_fn;
//
//
//     core->instruction_functions[bbVInstruction_setGoalMovable-bbInstruction_numTypes]
//         = bbVInstruction_setGoalMovable_fn;
//
//
//     core->instruction_functions[bbVInstruction_unsetGoalMovable-bbInstruction_numTypes]
//         = bbVInstruction_unsetGoalMovable_fn;
//
//     core->instruction_functions[bbVInstruction_updateAgentSquare-bbInstruction_numTypes]
//         = bbVInstruction_updateAgentSquare_fn;
//
//     core->instruction_functions[bbVInstruction_unupdateAgentSquare-bbInstruction_numTypes]
//         = bbVInstruction_unupdateAgentSquare_fn;
//
//     core->instruction_functions[bbVInstruction_updateAgentsSquare-bbInstruction_numTypes]
//         = bbVInstruction_updateAgentsSquare_fn;
//
//     core->instruction_functions[bbVInstruction_unupdateAgentsSquare-bbInstruction_numTypes]
//         = bbVInstruction_unupdateAgentsSquare_fn;
//
//
//     core->instruction_functions[bbVInstruction_updateAgent-bbInstruction_numTypes]
//         = bbVInstruction_updateAgent_fn;
//
//     core->instruction_functions[bbVInstruction_unupdateAgent-bbInstruction_numTypes]
//         = bbVInstruction_unupdateAgent_fn;
//
//     core->instruction_functions[bbVInstruction_updateAgents-bbInstruction_numTypes]
//         = bbVInstruction_updateAgents_fn;
//
//     core->instruction_functions[bbVInstruction_unupdateAgents-bbInstruction_numTypes]
//         = bbVInstruction_unupdateAgents_fn;
//
//     core->instruction_functions[bbVInstruction_commandAgent-bbInstruction_numTypes]
//         = bbVInstruction_commandAgent_fn;
//
//     core->instruction_functions[bbVInstruction_spawnUnitIn-bbInstruction_numTypes]
//     = bbVInstruction_spawnUnitIn_fn;
//     core->instruction_functions[bbVInstruction_spawnUnitOut-bbInstruction_numTypes]
//     = bbVInstruction_spawnUnitOut_fn;
//     core->instruction_functions[bbVInstruction_commandAgentMapClick-bbInstruction_numTypes]
//     = bbVInstruction_commandAgentMapClick_fn;
//
//     core->instruction_functions[bbVInstruction_damageAgent-bbInstruction_numTypes]
//     = bbVInstruction_damageAgent_fn;
//     core->instruction_functions[bbVInstruction_undamageAgent-bbInstruction_numTypes]
//     = bbVInstruction_undamageAgent_fn;
//
//
//
//     core->instruction_functions[bbVInstruction_spawnAgent-bbInstruction_numTypes]
//     = bbVInstruction_spawnAgent_fn;
//     core->instruction_functions[bbVInstruction_unspawnAgent-bbInstruction_numTypes]
//     = bbVInstruction_unspawnAgent_fn;
//
//     core->instruction_functions[bbVInstruction_deleteEntity-bbInstruction_numTypes]
//     = bbVInstruction_deleteEntity_fn;
//     core->instruction_functions[bbVInstruction_undeleteEntity-bbInstruction_numTypes]
//     = bbVInstruction_undeleteEntity_fn;
//
//
//     return bbSuccess;
// }