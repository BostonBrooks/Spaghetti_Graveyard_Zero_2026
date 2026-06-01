

#include "games/game0/maps/skellychase/core/instructions.h"
#include "engine/entities/bbAgents.h"
#include "engine/logic/bbList.h"



///Sort through agents and update square if necessary
bbFlag bbCoreInput_updateAgentsSquare(bbCore* core, bbAgents* agents,bbInstruction_source source, bbHandle action)
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
    instruction->data.agent_square.square_coords = SC;
    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;
}

bbFlag bbCoreInput_updateAgents(bbCore* core, bbAgents* agents,bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->source = source;
    instruction->redo_instruction = action;

    instruction->type = bbVInstruction_updateAgents;
    bbList_pushL(&core->do_stack, instruction);
}

bbFlag bbCoreInput_updateAgent(bbCore* core, bbHandle agent,bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->source = source;
    instruction->redo_instruction = action;
    instruction->data.agent_square.agent = agent;

    instruction->type = bbVInstruction_updateAgent;
    bbList_pushL(&core->do_stack, instruction);
}


bbFlag bbCoreInput_commandAgent_setGoalPoint(bbCore* core, bbHandle agent, bbMapCoords MC,bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->source = source;
    instruction->redo_instruction = action;
    instruction->data.agent_MC.agent = agent;
    instruction->data.agent_MC.coords = MC;


    instruction->type = bbVInstruction_commandAgent;
    bbList_pushL(&core->do_stack, instruction);
}

bbFlag bbCoreInput_spawnUnitOut(bbCore* core, I32 unit_type, bbMapCoords MC,bbMapCoords MC2, U64 time,
bbInstruction_source source, bbHandle action)
{

        bbInstruction* instruction;
        bbList_alloc(&core->do_stack, (void**) &instruction);
        instruction->type = bbVInstruction_spawnUnitOut;
        instruction->data.spawn_unit.type = unit_type;
        instruction->data.spawn_unit.position = MC;
        instruction->data.spawn_unit.goal_point = MC2;
        instruction->act_time = time;
        bbList_pushL(&core->do_stack, instruction);


        return bbSuccess;

}

bbFlag bbCoreInput_spawnUnitIn(bbCore* core, bbMapCoords MC,bbMapCoords goalcoords,I32 type_index, I32 entity_index,
    I32 moveable_index, U64 time, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbVInstruction_spawnUnitIn;
    instruction->data.spawn_unit.position = MC;
    instruction->data.spawn_unit.goal_point = goalcoords;
    instruction->data.spawn_unit.entity = entity_index;
    instruction->data.spawn_unit.moveable = moveable_index;
    instruction->data.spawn_unit.type =  type_index;
    instruction->act_time = time;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}


bbFlag bbCoreInput_damageAgent(bbCore* core, bbHandle agent, I32 hitpoints,bbInstruction_source source, bbHandle action){
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbVInstruction_damageAgent;
    instruction->data.damage_agent.agent = agent;
    instruction->data.damage_agent.hitpoints = hitpoints;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}