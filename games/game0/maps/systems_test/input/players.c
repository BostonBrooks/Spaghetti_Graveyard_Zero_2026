
#include "AI_system/player_send_goalpoint.h"
#include "core/actions.h"
#include "core/action_request.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/players/bbPlayers.h"
#include "engine/logic/bbFlag.h"

bbFlag bbPlayer_ClickMap_null(bbPlayers* players, bbMapCoords coords, U64 control_keys) ;
bbFlag bbPlayer_ClickUnit_null(bbPlayers* players, bbHandle entity_handle, U64 control_keys) ;
bbFlag bbPlayer_KeyPress_null(bbPlayers* players, U64 key, U64 control_keys);


bbFlag bbPlayer_ClickMap_default(bbPlayers* players, bbMapCoords coords, U64 control_keys);
bbFlag bbPlayer_ClickUnit_default(bbPlayers* players, bbHandle entity_handle, U64 control_keys) ;
bbFlag bbPlayer_KeyPress_default(bbPlayers* players, U64 key, U64 control_keys);


bbFlag bbPlayers_populate(bbPlayers* system) {
    system->states[bbPlayer_stateNULL].click_map = bbPlayer_ClickMap_null;
    system->states[bbPlayer_stateNULL].click_unit = bbPlayer_ClickUnit_null;
    system->states[bbPlayer_stateNULL].key_press = bbPlayer_KeyPress_null;

    bbStr_setStr(
        system->states[bbPlayer_stateNULL].bbInputMode_key,
        "TEST_INPUT_MODE", KEY_LENGTH);

    system->states[bbPlayer_stateDefault].click_map = bbPlayer_ClickMap_default;
    system->states[bbPlayer_stateDefault].click_unit = bbPlayer_ClickUnit_default;
    system->states[bbPlayer_stateDefault].key_press = bbPlayer_KeyPress_default;

    bbStr_setStr(
        system->states[bbPlayer_stateDefault].bbInputMode_key,
        "TEST_INPUT_MODE", KEY_LENGTH);


}


bbFlag bbPlayer_ClickMap_null(bbPlayers* players, bbMapCoords coords, U64 control_keys) {
    bbNotImplemented()

    bbDebug("Player clicked map coords (%d, %d, %d)), control keys:\n %064" PRIb64 "\n",
            coords.i,
            coords.j,
            coords.k,
            control_keys);
}
bbFlag bbPlayer_ClickUnit_null(bbPlayers* players, bbHandle entity_handle, U64 control_keys) {
    bbDebug("Player clicked entity index %d, control keys:\n %064" PRIb64 "\n",
    entity_handle.system.index,
    control_keys);
}
bbFlag bbPlayer_KeyPress_null(bbPlayers* players, U64 key, U64 control_keys) {
    bbDebug("Player clicked key %llu, control keys:\n %064" PRIb64 "\n",
    key,control_keys);
}


bbFlag bbPlayer_ClickMap_default(bbPlayers* players, bbMapCoords coords, U64 control_keys) {

    I32 this_player_int = players->this_player;
    bbPlayer* this_player = &players->players[this_player_int];


    bbCoreInput_sendAIGoalpoint(&home.core.core,
                                 this_player->selected_entities[0],
                                 coords,
                                 home.core.core.actual_time,
                                 bbInstructionSource_internal,
                                 no_handle);

    return bbSuccess;
}

extern U32 collision;
bbFlag bbPlayer_ClickUnit_default(bbPlayers* players, bbHandle entity_handle, U64 control_keys) {


    bbTeam* team;
    bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS,entity_handle,bbECS_Teams,NULL,(bbComponent**)&team);

    if (team!=NULL) {
        if (team->team == bbTeam_player) {
bbHere()
            bbHandle server_handle;
            bbServerEntity* server_entity;
            bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS,entity_handle,bbECS_ServerEntities,&server_handle,(bbComponent**)&server_entity);

            if (server_entity!=NULL) {
                bbActionRequest_setPlayerEntity(&home.core.core,
                                               home.ECS.players.this_player,
                                               collision++,
                                               home.core.core.actual_time,
                                               home.core.core.actual_time,
                                               home.ECS.players.this_player,
                                               server_handle);
            }
        } else {

            bbHandle server_handle;
            bbServerEntity* server_entity;
            bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS,entity_handle,bbECS_ServerEntities,&server_handle,(bbComponent**)&server_entity);

            if (server_entity!=NULL) {
                bbHere()
                bbAction action;
                action.header.type = bbActionType_setTarget;
                action.header.status = bbAction_Wait;
                action.header.sender = players->this_player;
                action.header.collision = collision++;
                action.header.created_tick = home.core.core.actual_time;
                action.header.act_tick = home.core.core.actual_time;
                bbStr_setStr(action.header.key,"ACHILLES", KEY_LENGTH);
                action.handle = server_handle;
                action.integer = players->this_player;

                bbCoreInput_requestAction(&home.core.core,&home.network,&action,home.core.core.actual_time,
                                            bbInstructionSource_internal, no_handle);
            }
        }
    }

    bbDebug("clicked entity index %d, control keys:\n %064" PRIb64 "\n",
        entity_handle.system.index,
        control_keys);
}
bbFlag bbPlayer_KeyPress_default(bbPlayers* players, U64 key, U64 control_keys) {
    bbDebug("Player clicked key %llu, control keys:\n %064" PRIb64 "\n",
    key,control_keys);
}