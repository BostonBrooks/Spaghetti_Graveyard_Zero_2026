

#include "games/game0/maps/skellychase/core/instructions.h"
#include "engine/agents/bbAgents2.h"
#include "engine/logic/bbList.h"



///Sort through agents and update square if necessary
bbFlag bbCoreInput_updateAgentsSquare(bbCore* core, bbAgents2* agents,bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->source = source;
    instruction->redo_instruction = action;

    instruction->type = bbVInstruction_updateAgentsSquare;
    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;
}

///If the agent has moved into a different square, add to new list
bbFlag bbCoreInput_updateAgentSquare(bbCore* core, bbHandle agent, bbSquareCoords SC,bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->source = source;
    instruction->redo_instruction = action;

    instruction->type = bbVInstruction_updateAgentSquare;
    instruction->data.agent_square.agent = agent;
    instruction->data.agent_square.square = SC;
    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;
}