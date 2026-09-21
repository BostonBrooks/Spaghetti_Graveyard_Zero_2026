//bbFlag bbAction_fn(bbCore* core, bbAction* action);

#include "engine/core/bbCore.h"
#include "engine/ECS/bbECS.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/moveables/bbMoveables.h"
#include "engine/ECS/moveables/bbMoveables_setState.h"
#include "engine/test_string/bbTestString.h"


bbFlag bbAction_setGoalpoint_fn(bbCore* core, bbAction* action)
{
    bbAI_Component* component;
    bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS, action->handle,bbECS_AI, NULL, (bbComponent**) &component);

    bbAI_CommandData data;
    data.goal_point = action->map_coords;

    bbAI_onCommand(component,
                          (bbAI_System*)home.ECS.ECS->systems[bbECS_AI],
                          bbAI_setGoalPoint,
                          data,
                          true);


    bbHandle action_handle;
    bbVPool_reverseLookup(core->action_pool,action,&action_handle);

    bbCI_doNothing(core, bbInstructionSource_action, action_handle);

    bbCore_react(core);
    return bbSuccess;
}

bbFlag bbAction_setTarget_fn(bbCore* core, bbAction* action)
{
    bbAI_Component* component;

    bbPlayers* players = &home.ECS.players;
    bbHandle player_character = players->players[players->this_player].selected_entities[0];

    bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS, player_character,bbECS_AI, NULL, (bbComponent**) &component);

    bbHandle server_handle = action->handle;
    bbHandle entity_handle;

    bbHandle_mapComponent(home.ECS.ECS,bbECS_ServerEntities,server_handle,bbECS_ECS,&entity_handle,NULL);

    bbAI_CommandData data;
    data.handle = entity_handle;

    bbAI_onCommand(component,
                          (bbAI_System*)home.ECS.ECS->systems[bbECS_AI],
                          bbAI_targetMonster,
                          data,
                          true);


    bbHandle action_handle;
    bbVPool_reverseLookup(core->action_pool,action,&action_handle);

    bbCI_doNothing(core, bbInstructionSource_action, action_handle);

    bbCore_react(core);
    return bbSuccess;
}
