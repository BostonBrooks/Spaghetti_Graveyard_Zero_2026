#ifndef  INSTRUCTIONS_H
#define  INSTRUCTIONS_H
#include "engine/core/bbInstruction.h"

typedef enum
{
    bbVInstruction_setGoalpointOut = bbInstruction_numTypes,
    bbVInstruction_setGoalpointIn,
    bbVInstruction_unsetGoalpoint,

    bbVInstruction_updateMoveables,
    bbVInstruction_unupdateMoveables,

    bbVInstruction_setGoalMoveable,
    bbVInstruction_unsetGoalMoveable,

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


    bbVInstruction_setAgentHP,
    bbVInstruction_unsetAgentHP,

    bbVInstruction_numTypes
}bbVInstruction_type;


bbFlag bbVInstruction_setGoalpointOut_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_setGoalpointIn_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unsetGoalpoint_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_updateMoveables_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateMoveables_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_setGoalMoveable_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unsetGoalMoveable_fn(bbCore* core, bbInstruction* instruction);



bbFlag bbVInstruction_updateMoveables_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateMoveables_fn(bbCore* core, bbInstruction* instruction);



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


bbFlag bbVInstruction_setAgentHP_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unsetAgentHP_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_commandAgentMapClick_fn(bbCore* core, bbInstruction* instruction);
#endif// INSTRUCTIONS_H