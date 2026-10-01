
#include "engine/ECS/players/bbPlayers_target.h"

#include "print_entity.h"
#include "core/actions.h"
#include "core/action_request.h"
#include "engine/core/bbAction.h"
#include "engine/core/bbCore.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbTerminal.h"
#include "engine/test_string/bbTestString.h"

bbFlag bbActionRequest_setPlayerTarget(void* Core,
                                       U32 sender,
                                       U32 collision,
                                       bbTime created_tick,
                                       bbTime act_tick,
                                       U32 player,
                                       bbHandle target_server_handle) {

    bbCore* core = (bbCore*)Core;

    bbAction action;
    action.header.type = bbActionType_setPlayerTarget;
    action.header.status = bbAction_Wait;
    action.header.sender = sender;
    action.header.collision = collision;
    action.header.created_tick = created_tick;
    action.header.act_tick = act_tick;
    action.integer = player;
    action.handle = target_server_handle;
    bbAction_request(core,&home.network,&action);

    return bbSuccess;
}

bbFlag bbAction_setPlayerTarget_fn(bbCore* core, bbAction* action) {
    bbNotImplemented()
    bbHandle server_handle = action->handle;
    bbHandle entity_handle;
    bbHandle_mapComponent(core->ECS, bbECS_ServerEntities, server_handle, bbECS_ECS, &entity_handle, NULL);

    bbEntity_print(entity_handle);
    bbHandle action_handle;
    bbVPool_reverseLookup(core->action_pool,action,&action_handle);
    bbCS_setPlayerTarget(core, action->integer, entity_handle, bbInstructionSource_action, action_handle);



    return bbSuccess;
}
bbFlag bbActionRequest_setPlayerAttack(void* Core,
                       U32 sender,
                       U32 collision,
                       bbTime created_tick,
                       bbTime act_tick,
                       U32 player,
                       bbHandle target_server_handle) {


        bbCore* core = (bbCore*)Core;

        bbAction action;
        action.header.type = bbActionType_setPlayerAttack;
        action.header.status = bbAction_Wait;
        action.header.sender = sender;
        action.header.collision = collision;
        action.header.created_tick = created_tick;
        action.header.act_tick = act_tick;
        action.integer = player;
        action.handle = target_server_handle;
        bbAction_request(core,&home.network,&action);

        return bbSuccess;

}

bbFlag bbAction_setPlayerAttack_fn(bbCore* core, bbAction* action) {

    U32 player = action->integer;
    bbHandle target_server_handle = action->handle;
    bbHandle action_handle;
    bbHandle target_handle;
    bbAI_Component* ai_component;
    bbHandle_mapComponent(core->ECS,
                   bbECS_ServerEntities,
                   target_server_handle,
                   bbECS_ECS,
                   &target_handle,NULL);

    bbHandle player_entity_handle = home.ECS.players.players[player].selected_entities[0];

    bbHandle_mapComponent(core->ECS, bbECS_ECS, player_entity_handle, bbECS_AI, NULL, (bbComponent**)&ai_component);

    bbAI_CommandData data;
    data.handle = target_handle;

    bbEntity_print(target_handle)
    bbEntity_print(home.ECS.players.players[player].target_entity)

    bbAI_onCommand(ai_component,
                          (bbAI_System*)home.ECS.ECS->systems[bbECS_AI],
                          bbAI_targetMonster,
                          data,
                          true);

    bbVPool_reverseLookup(core->action_pool,action,&action_handle);

    bbCI_doNothing(core, bbInstructionSource_action, action_handle);

    return bbSuccess;
}




bbFlag bbCI_setPlayerTarget(bbCore* core, U32 player, bbHandle entity_handle, bbInstruction_source source, bbHandle action)
{
    bbNotImplemented()
    return bbSuccess;
}
bbFlag bbCS_setPlayerTarget(bbCore* core, U32 player, bbHandle entity_handle, bbInstruction_source source, bbHandle action)
{
    bbNotImplemented() //rollback

    bbPlayer* player_object = &home.ECS.players.players[player];
    player_object->target_entity = entity_handle;


    bbEntity_print(entity_handle);

    return bbSuccess;
}

bbFlag bbI_setPlayerTarget_fn(bbCore* core, bbInstruction* instruction)
{
    bbNotImplemented()
    return bbSuccess;
}
bbFlag bbI_unsetPlayerTarget_fn(bbCore* core, bbInstruction* instruction)
{
    bbNotImplemented()
    return bbSuccess;
}