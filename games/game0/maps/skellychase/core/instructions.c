
#include "instructions.h"

#include "core_inputs.h"
#include "engine/core/bbAction.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/logic/bbFlag.h"
#include "engine/core/bbInstruction.h"
#include "engine/data/bbHome.h"
#include "engine/network/bbNetworkApp.h"


extern U32 collision;


bbFlag bbVInstruction_setGoalpointOut_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_setGoalpointOut(&home.network, instruction->data.banana.entity, instruction->data.banana.position, instruction->act_time, collision++);
    return bbSuccess;
}

bbFlag bbVInstruction_setGoalpointIn_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbVInstruction_unsetGoalpoint;
    //undo_instruction->data.map_coords = home.agents_app.agents.agents[instruction->player].goalpoint;
    undo_instruction->source = instruction->source;
    undo_instruction->player = instruction->player;


    bbMapCoords MC = instruction->data.map_coords;
    bbHandle handle; handle.ptr = &MC;
    /*bbAgent2_onCommand(home.agents_app.player,
                          home.agents_app.agents,
                          bbAC_setGoalPoint,
                          handle);*/

    bbAgent* player;
    //TODO player_entity_int should be set as an argument to bbCoreInput_setGoalpointOut
    I32 player_entity_int = instruction->player;
    bbEntity* player_entity = &home.agents_app.entities.entity[player_entity_int];
    bbVPool_lookup(home.agents_app.agents->pool,(void**)&player,player_entity->agent);

    bbMoveable* moveable = &home.agents_app.movables.moveables[player_entity_int];
    undo_instruction->data.map_coords = moveable->goalpoint;
    //bbVPool_reverseLookup(home.agents_app.agents->pool, (void*)player, &handle);

    bbAgentCommandData data;
    data.moveable = 0;
    data.goal_point = MC;
    data.type = bbMoveableType_GoalPoint;

    //TODO use bbAgent_Command_Player

    bbAgent2_onCommand(player, home.agents_app.agents,0,data);


    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()
    return bbSuccess;
}


bbFlag bbVInstruction_unsetGoalpoint_fn(bbCore* core, bbInstruction* instruction)
{
    //home.agents_app.agents.agents[instruction->player].goalpoint = instruction->data.map_coords;

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    bbNotHere()
}

bbFlag bbVInstruction_updateMoveables_fn(bbCore* core, bbInstruction* instruction)
{


    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbVInstruction_unupdateMoveables;
    undo_instruction->source = instruction->source;

    bbMoveables_snapshot* snapshot;
    bbHandle snapshot_handle;
    bbVPool_alloc(home.agents_app.movables.snapshots, (void**)&snapshot);

    bbVPool_reverseLookup(home.agents_app.movables.snapshots, (void*)snapshot, &snapshot_handle);

    undo_instruction->snapshot = snapshot_handle;

    for (I32 i = 0; i < NUM_MOVEABLES; i++)
    {
        snapshot->moveables[i].position = home.agents_app.movables.moveables[i].position;
        snapshot->moveables[i].goalpoint = home.agents_app.movables.moveables[i].goalpoint;
    }



    bbMoveables_update(&home.agents_app.movables);

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()
}

bbFlag bbVInstruction_unupdateMoveables_fn(bbCore* core, bbInstruction* instruction)
{

    bbMoveables_snapshot* snapshot;
    bbVPool_lookup(home.agents_app.movables.snapshots, (void**)&snapshot, instruction->snapshot);

    for (I32 i = 0; i < NUM_MOVEABLES; i++)
    {
        home.agents_app.movables.moveables[i].position = snapshot->moveables[i].position;
        home.agents_app.movables.moveables[i].goalpoint = snapshot->moveables[i].goalpoint;
    }

    bbVPool_free(home.agents_app.movables.snapshots, snapshot);

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }


    bbNotHere()
}

bbFlag bbVInstruction_setGoalMoveable_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);

    bbMoveable* moveable = &home.agents_app.movables.moveables[
        instruction->data.moveable_goal.subject_moveable];

    undo_instruction->type = bbVInstruction_unsetGoalMoveable;
    undo_instruction->source = instruction->source;
    undo_instruction->player = instruction->player;
    undo_instruction->data.moveable_goal.subject_moveable =
        instruction->data.moveable_goal.subject_moveable;
    undo_instruction->data.moveable_goal.goal_moveable = moveable->goal_moveable;
    undo_instruction->data.moveable_goal.goal_coords = moveable->goalpoint;
    undo_instruction->data.moveable_goal.type = moveable->type;


    moveable->goal_moveable = instruction->data.moveable_goal.goal_moveable;
    moveable->goalpoint = instruction->data.moveable_goal.goal_coords;
    moveable->type = instruction->data.moveable_goal.type;


    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()
    return bbSuccess;
}

bbFlag bbVInstruction_unsetGoalMoveable_fn(bbCore* core, bbInstruction* instruction)
{

    bbMoveable* moveable = &home.agents_app.movables.moveables[
        instruction->data.moveable_goal.subject_moveable];

    moveable->goal_moveable = instruction->data.moveable_goal.goal_moveable;
    moveable->goalpoint = instruction->data.moveable_goal.goal_coords;
    moveable->type = instruction->data.moveable_goal.type;


    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    bbNotHere()
}

bbFlag bbVInstruction_updateAgentSquare_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);

    bbAgent* agent;
    bbVPool_lookup(home.agents_app.agents->pool, (void**)&agent,
        instruction->data.agent_square.agent);

    bbSquareCoords old_square_coords = agent->square_coords;
    bbSquareCoords new_square_coords = instruction->data.agent_square.square;

    undo_instruction->source = instruction->source;
    undo_instruction->type = bbVInstruction_unupdateAgentSquare;
    undo_instruction->data.agent_square.agent = instruction->data.agent_square.agent;
    undo_instruction->data.agent_square.square = old_square_coords;

    bbAgents_square* old_square = bbAgents_getSquare(home.agents_app.agents,
    old_square_coords.i, old_square_coords.j);

    bbAgents_square* new_square = bbAgents_getSquare(home.agents_app.agents,
        new_square_coords.i, new_square_coords.j);

    bbList_remove(&old_square->agents,agent);
    agent->square_coords = new_square_coords;
    bbList_pushL(&new_square->agents,agent);

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()
    return bbSuccess;
}

bbFlag bbVInstruction_unupdateAgentSquare_fn(bbCore* core, bbInstruction* instruction)
{
bbHere()
    bbAgent* agent;
    bbVPool_lookup(home.agents_app.agents->pool, (void**)&agent,
        instruction->data.agent_square.agent);

    bbSquareCoords old_square_coords = agent->square_coords;
    bbSquareCoords new_square_coords = instruction->data.agent_square.square;

    bbAgents_square* old_square = bbAgents_getSquare(home.agents_app.agents,
    old_square_coords.i, old_square_coords.j);

    bbAgents_square* new_square = bbAgents_getSquare(home.agents_app.agents,
        new_square_coords.i, new_square_coords.j);

    bbList_remove(&old_square->agents,agent);
    agent->square_coords = new_square_coords;
    bbList_pushL(&new_square->agents,agent);

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    bbNotHere()
}

//typedef bbFlag bbListFunction(bbList* list, void* node, void* cl);
bbFlag updateAgentSquare_list_fn(bbList* list, void* node, void* cl)
{

    bbAgent* agent = (bbAgent*)node;
    bbSquareCoords old_square_coords = agent->square_coords;
    bbMoveable* moveable = &home.agents_app.movables.moveables[agent->moveable];
    bbSquareCoords new_square_coords = bbMapCoords_getSquareCoords(moveable->position);

    if (old_square_coords.i != new_square_coords.i || old_square_coords.j != new_square_coords.j)
    {
        bbHandle handle;
        bbVPool_reverseLookup(list->pool, (void*)node, &handle);
        bbCoreInput_updateAgentSquare(&home.core.core, handle, new_square_coords,bbInstructionSource_internal, no_handle);
    }
    return bbContinue;
}

bbFlag bbVInstruction_updateAgentsSquare_fn(bbCore* core, bbInstruction* instruction)
{

    bbList_mapL(&home.agents_app.agents->full_list, updateAgentSquare_list_fn, NULL);

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbVInstruction_unupdateAgentsSquare;
    undo_instruction->source = instruction->source;

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    return bbSuccess;
}
bbFlag bbVInstruction_unupdateAgentsSquare_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    return bbSuccess;
}

bbFlag updateAgent_list_fn(bbList* list, void* node, void* cl)
{
    bbHandle handle;
    bbVPool_reverseLookup(list->pool,node,&handle);
    bbCoreInput_updateAgent(&home.core.core, handle, bbInstructionSource_input,
                            no_handle);

    return bbContinue;
}

bbFlag bbVInstruction_updateAgents_fn(bbCore* core, bbInstruction* instruction)
{

    bbList_mapL(&home.agents_app.agents->full_list, updateAgent_list_fn, NULL);

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbVInstruction_unupdateAgents;
    undo_instruction->source = instruction->source;

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    return bbSuccess;
}
bbFlag bbVInstruction_unupdateAgents_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    return bbSuccess;
}

bbFlag bbVInstruction_updateAgent_fn(bbCore* core, bbInstruction* instruction)
{


    bbAgent* agent;
    bbVPool_lookup(home.agents_app.agents->pool, (void**)&agent,
        instruction->data.agent_square.agent);

    bbAgent2_update(agent);

    //No undo instruction because this does not directly modify data
    bbVPool_free(core->instruction_pool, (void*)instruction);
    return bbSuccess;
}
bbFlag bbVInstruction_unupdateAgent_fn(bbCore* core, bbInstruction* instruction)
{
    bbHere()
    return bbSuccess;
}


bbFlag bbVInstruction_commandAgent_fn(bbCore* core, bbInstruction* instruction)
{
    bbAgent* agent;
    bbVPool_lookup(home.agents_app.agents->pool, (void**)&agent,
        instruction->data.agent_square.agent);

    bbAgent* player;
    I32 player_entity_int = home.agents_app.player_entity;
    bbEntity* player_entity = &home.agents_app.entities.entity[player_entity_int];
    bbVPool_lookup(home.agents_app.agents->pool,(void**)&player,player_entity->agent);

    bbHandle handle; handle.ptr = &instruction->data.agent_MC.map_coords;
    bbAgent2_onCommand(player,
                          home.agents_app.agents,
                          bbAC_setGoalPoint,
                          instruction->data.agent_command);

    //No undo instruction because this does not directly modify data
    bbVPool_free(core->instruction_pool, (void*)instruction);
    return bbSuccess;
}

bbFlag bbVInstruction_setAgentHP_fn(bbCore* core, bbInstruction* instruction)
{

    bbAgent* agent;
    bbVPool_lookup(home.agents_app.agents->pool, (void**)&agent,
        instruction->data.unspawn.agent);
    I32 hitpoints = instruction->data.unspawn.entity;


    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbVInstruction_unsetAgentHP;
    undo_instruction->source = instruction->source;
    undo_instruction->data.unspawn.agent = instruction->data.unspawn.agent;
    undo_instruction->data.unspawn.entity = agent->health;

    agent->health = hitpoints;

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    return bbSuccess;
}
bbFlag bbVInstruction_unsetAgentHP_fn(bbCore* core, bbInstruction* instruction)
{
    return bbSuccess;
}

bbFlag bbVInstruction_spawnUnitIn_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbVInstruction_unspawnUnit;
    //undo_instruction->data.map_coords = home.agents_app.agents.agents[instruction->player].goalpoint;
    undo_instruction->source = instruction->source;
    undo_instruction->player = instruction->player;
    undo_instruction->data.unspawn.entity = instruction->data.unit.entity;
    undo_instruction->data.unspawn.moveable = instruction->data.unit.moveable;

    bbAgent* agent;

    bbDebug("type = %d\n",instruction->data.unit.type);

    //bbAgent_newBanana(home.agents_app.agents,&agent, instruction->data.banana.position,
    //    instruction->data.banana.entity, instruction->data.banana.moveable);
    bbSpawner_spawnEntityI(&home.spawner,
                            &agent,
                            instruction->data.unit.position,
                            instruction->data.unit.goalpoint,
                            instruction->data.unit.moveable,
                            instruction->data.unit.entity,
                            instruction->data.unit.type);


    bbHandle agent_handle;
    bbVPool_reverseLookup(home.agents_app.agents->pool, agent, &agent_handle);

    undo_instruction->data.unspawn.agent = agent_handle;



    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()
    return bbSuccess;
}

bbFlag bbVInstruction_spawnUnitOut_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_spawnUnitOut(&home.network, instruction->data.unit.type,
        instruction->data.unit.position, instruction->data.unit.goalpoint, instruction->act_time, collision++);
    return bbSuccess;
}


bbFlag bbVInstruction_commandAgentMapClick_fn(bbCore* core, bbInstruction* instruction)
{
    bbMapCoords MC = instruction->data.banana.position;
    I32 player_entity =  instruction->data.banana.entity;
    I32 button = instruction->data.banana.moveable;

    bbEntity* entity = &home.agents_app.entities.entity[player_entity];
    bbAgent* agent;
    bbVPool_lookup(home.agents_app.agents->pool,(void**)&agent,entity->agent);

    bbAgentCommandData data;
    data.goal_point = MC;
    data.moveable = button;
    bbAgent2_onCommand(agent,home.agents_app.agents,bbAC_mapClick,data);
    return bbSuccess;
}