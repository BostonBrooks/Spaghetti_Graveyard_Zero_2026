#ifndef  INSTRUCTIONS_H
#define  INSTRUCTIONS_H
#include "engine/core/bbInstruction.h"

typedef enum
{
    bbVInstruction_setGoalpointOut = bbInstruction_numTypes,
    bbVInstruction_setGoalpointIn,
    bbVInstruction_unsetGoalpoint,

    bbVInstruction_updateMovables,
    bbVInstruction_unupdateMovables,

    bbVInstruction_setGoalMovable,
    bbVInstruction_unsetGoalMovable,

    bbVInstruction_updateAgentSquare,
    bbVInstruction_unupdateAgentSquare,

    bbVInstruction_updateAgentsSquare,
    bbVInstruction_unupdateAgentsSquare,

    bbVInstruction_updateAgent,
    bbVInstruction_unupdateAgent,

    bbVInstruction_updateAgents,
    bbVInstruction_unupdateAgents,

    bbVInstruction_commandAgent,

    bbVInstruction_spawnUnitOut,
    bbVInstruction_spawnUnitIn,
    bbVInstruction_unspawnUnit,
    bbVInstruction_commandAgentMapClick,


    bbVInstruction_damageAgent,
    bbVInstruction_undamageAgent,



    bbVInstruction_spawnAgent,
    bbVInstruction_unspawnAgent,



    bbVInstruction_deleteEntity,
    bbVInstruction_undeleteEntity,

    bbVInstruction_numTypes
}bbVInstruction_type;


bbFlag bbVInstruction_setGoalpointOut_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_setGoalpointIn_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unsetGoalpoint_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_updateMovables_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateMovables_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_setGoalMovable_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unsetGoalMovable_fn(bbCore* core, bbInstruction* instruction);



bbFlag bbVInstruction_updateMovables_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateMovables_fn(bbCore* core, bbInstruction* instruction);



bbFlag bbVInstruction_updateAgentsSquare_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateAgentsSquare_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_updateAgentSquare_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateAgentSquare_fn(bbCore* core, bbInstruction* instruction);



bbFlag bbVInstruction_updateAgents_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateAgents_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_updateAgent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateAgent_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbVInstruction_commandAgent_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbVInstruction_spawnUnitOut_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_spawnUnitIn_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unspawnUnit_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbVInstruction_damageAgent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_undamageAgent_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbVInstruction_spawnAgent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unspawnAgent_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_commandAgentMapClick_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_deleteEntity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_undeleteEntity_fn(bbCore* core, bbInstruction* instruction);

#endif// INSTRUCTIONS_H