

#include <stdlib.h>

#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/logic/bbFlag.h"
#include "instructions.h"

bbFlag bbCore_initVInstructions(bbCore* core)
{
    core->instruction_functions
        = calloc(bbVInstruction_numTypes-bbInstruction_numTypes,
            sizeof (bbInstruction_fn*));

    core->instruction_functions[bbVInstruction_setGoalpointOut-bbInstruction_numTypes]
        = bbVInstruction_setGoalpointOut_fn;

    core->instruction_functions[bbVInstruction_setGoalpointIn-bbInstruction_numTypes]
        = bbVInstruction_setGoalpointIn_fn;

    core->instruction_functions[bbVInstruction_unsetGoalpoint-bbInstruction_numTypes]
        = bbVInstruction_unsetGoalpoint_fn;

    core->instruction_functions[bbVInstruction_updateMoveables-bbInstruction_numTypes]
        = bbVInstruction_updateMoveables_fn;

    core->instruction_functions[bbVInstruction_unupdateMoveables-bbInstruction_numTypes]
        = bbVInstruction_unupdateMoveables_fn;


    core->instruction_functions[bbVInstruction_setGoalMoveable-bbInstruction_numTypes]
        = bbVInstruction_setGoalMoveable_fn;


    core->instruction_functions[bbVInstruction_unsetGoalMoveable-bbInstruction_numTypes]
        = bbVInstruction_unsetGoalMoveable_fn;

    core->instruction_functions[bbVInstruction_updateAgentSquare-bbInstruction_numTypes]
        = bbVInstruction_updateAgentSquare_fn;

    core->instruction_functions[bbVInstruction_unupdateAgentSquare-bbInstruction_numTypes]
        = bbVInstruction_unupdateAgentSquare_fn;

    core->instruction_functions[bbVInstruction_updateAgentsSquare-bbInstruction_numTypes]
        = bbVInstruction_updateAgentsSquare_fn;

    core->instruction_functions[bbVInstruction_unupdateAgentsSquare-bbInstruction_numTypes]
        = bbVInstruction_unupdateAgentsSquare_fn;


    core->instruction_functions[bbVInstruction_updateAgent-bbInstruction_numTypes]
        = bbVInstruction_updateAgent_fn;

    core->instruction_functions[bbVInstruction_unupdateAgent-bbInstruction_numTypes]
        = bbVInstruction_unupdateAgent_fn;

    core->instruction_functions[bbVInstruction_updateAgents-bbInstruction_numTypes]
        = bbVInstruction_updateAgents_fn;

    core->instruction_functions[bbVInstruction_unupdateAgents-bbInstruction_numTypes]
        = bbVInstruction_unupdateAgents_fn;


    core->instruction_functions[bbVInstruction_commandAgent-bbInstruction_numTypes]
        = bbVInstruction_commandAgent_fn;
    return bbSuccess;
}