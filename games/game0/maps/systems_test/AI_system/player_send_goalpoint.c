#include "games/game0/maps/systems_test/AI_system/player_send_goalpoint.h"

#include "core/actions.h"
#include "core/action_request.h"
#include "core/instructions.h"
#include "engine/core/bbInstruction_operations.h"
#include "engine/data/bbHome.h"
#include "engine/test_string/bbTestString.h"

bbFlag bbCoreInput_sendAIGoalpoint(bbCore* core,
                                 bbHandle entity,
                                 bbMapCoords MC,
                                 bbTime time,
                                 bbInstruction_source source,
                                 bbHandle action)
{bbHere()
    allocActiveInstruction(instruction)
    instruction->type = bbI_AI_sendGoalpoint;
    instruction->data.agent_MC.handle1 = entity;
    instruction->data.agent_MC.coords = MC;
    instruction->act_time = time;
    pushActiveInstruction(instruction)
    return bbSuccess;
}


extern U32 collision;
bbFlag bbInstruction_sendAIGoalpoint_fn(bbCore* core, bbInstruction* instruction) {
bbHere()
    bbMapCoords MC = instruction->data.agent_MC.coords;
    bbHandle entity_handle = instruction->data.agent_MC.handle1;
    bbHandle server_handle;
    bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS,entity_handle,bbECS_ServerEntities,&server_handle,NULL);
    bbNetworkApp_sendAIGoalpoint(&home.network, server_handle,MC, instruction->act_time, collision++);



}

bbFlag bbNetworkApp_sendAIGoalpoint(bbNetwork* Network, bbHandle server_entity, bbMapCoords MC, bbTime time, U32 collision)
{bbHere()
    bbAction action;
    action.header.type = bbActionType_sendAIGoalpoint;
    action.header.status = bbAction_Wait;
    action.header.sender = home.ECS.players.this_player;
    action.header.collision = collision;
    action.header.created_tick = time;
    action.header.act_tick = time;
    action.handle = server_entity;
    action.goal_coords = MC;
    bbAction_request(&home.core.core,&home.network,&action);

    return bbSuccess;
}

bbFlag bbAction_setAIGoalpoint_fn(bbCore* core, bbAction* action) {
    bbHere()
    bbMapCoords MC = action->goal_coords;
    bbHandle server_handle = action->handle;

    bbAI_Component* component;
    bbHandle_mapComponent(home.ECS.ECS,
        bbECS_ServerEntities,
        server_handle,
        bbECS_AI,
        NULL,
        (bbComponent**) &component);

    bbAI_CommandData data;
    data.goal_point = MC;

    bbHandle entity_handle;
    bbECS_entity*  entity;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_ECS, &entity_handle,
                             (bbComponent**)&entity);
    bbDebug("ai set goalpoint %s\n", entity->key);
    bbAI_onCommand(component,
                      (bbAI_System*)home.ECS.ECS->systems[bbECS_AI],
                      bbAI_setGoalPoint,
                      data,
                      true);

    bbHandle action_handle;
    bbVPool_reverseLookup(core->action_pool,action,&action_handle);

    bbCI_doNothing(core, bbInstructionSource_action, action_handle);

    bbECS_entity* entity2;
    bbHandle_mapComponent(home.ECS.ECS,
    bbECS_ServerEntities,
    server_handle,
    bbECS_ECS,
    NULL,
    (bbComponent**) &entity2);

    bbDebug("commanded %s to set goalpoint\n", entity2->key);

    return bbSuccess;

}