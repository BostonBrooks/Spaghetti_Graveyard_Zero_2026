
#include "engine/ECS/players/bbPlayers_target.h"
#include "engine/logic/bbTerminal.h"
#include "games/game0/maps/systems_test/print_entity.h"

extern U32 collision;

bbFlag bbPlayer_ClickMap_requestingMaths(bbPlayers* players, bbMapCoords coords, U64 control_keys, bbVPMouseType type) {

    I32 this_player_int = players->this_player;
    bbPlayer* this_player = &players->players[this_player_int];
    //bbHere()
        if (type == VPMouseRightDown) {
            //bbHere()
            bbSpawnFunctionArgs args;
            args.state = 0;
            args.speed = 15000;
            args.radius = 200000;
            args.mass = 1000;
            args.random_seed = 193;
            args.position = coords;
            args.goalpoint = coords;
            args.handle = no_handle;
            args.goal_handle = no_handle;
            bbCI_live_spawnEntity(&home.core.core, args, "SKELLY_LIVE", bbInstructionSource_internal, no_handle);
        } else {
            //bbDebug("this_player->state = %d\n", this_player->state);

            //bbVPMouseType_print(type)
            if (type == VPMouseLeftDown || type == VPMouseLeftDrag) {

                bbCoreInput_sendAIGoalpoint(&home.core.core,
                                        this_player->selected_entities[0],
                                        coords,
                                        home.core.core.actual_time,
                                        bbInstructionSource_internal,
                                        no_handle);


            }
        }
    return bbSuccess;
}
bbFlag bbPlayer_ClickUnit_requestingMaths(bbPlayers* players, bbHandle entity_handle, U64 control_keys, bbVPMouseType type){

    bbEntity_print(entity_handle);

    if (type == VPMouseLeftDown) {
        bbHandle target_server_handle;

        bbFlag flag = bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS,entity_handle,bbECS_ServerEntities,&target_server_handle,NULL);
        if (flag != bbSuccess) {
           // bbDebug("server handle not found\n");
            return bbSuccess;
        }

        bbECS_entity* entity;
        bbHandle_getComponent(&home.ECS.ECS->system,(bbComponent**)&entity,entity_handle);
        //bbDebug("player sets target %s\n", entity->key);

        bbActionRequest_setPlayerTarget(&home.core.core,
                                        home.ECS.players.this_player,
                                        collision++,
                                        home.core.core.actual_time,
                                        home.core.core.actual_time,
                                        home.ECS.players.this_player,
                                        target_server_handle);


        //bbDebug("bbVPMouseType type = %d\n", type);
        bbTextInput* text_input;
        bbHandle widget_handle;
        bbDictionary_lookup(home.UI.widgets.dict, "TEXT_INPUT", &widget_handle);
        bbWidget* widget;
        bbVPool_lookup(home.UI.widgets.pool,(void**)&widget,widget_handle);

        text_input = widget->extra_data;

        bbMutexLock(&text_input->mutex);

        I32 input_number = bbStr_toI32(text_input->raw_buffer);

        bbMutexUnlock(&text_input->mutex);
        bbTextInput_setStr(text_input,"");

        //bbDebug("input number = %d\n", input_number);
        U64 rand = bbRand();

        bbActionRequest_askQuestion(&home.core.core,
                                            home.ECS.players.this_player,
                                            collision++,
                                            home.core.core.actual_time,
                                            home.core.core.actual_time,
                                            home.ECS.players.this_player,
                                            0,
                                      rand);


    }
    return bbSuccess;
}
bbFlag bbPlayer_KeyPress_requestingMaths(bbPlayers* players, U64 key, U64 control_keys){
    {
        //bbDebug("Player clicked key %llu, control keys:\n %064" PRIb64 "\n",
        //key,control_keys);

        if (key == 0) {

        }

        return bbSuccess;
    }
}


bbFlag bbPlayer_ClickMap_answeringMaths(bbPlayers* players, bbMapCoords coords, U64 control_keys, bbVPMouseType type){

    I32 this_player_int = players->this_player;
    bbPlayer* this_player = &players->players[this_player_int];
    //bbHere()
        if (type == VPMouseRightDown) {
            //bbHere()
            bbSpawnFunctionArgs args;
            args.state = 0;
            args.speed = 15000;
            args.radius = 200000;
            args.mass = 1000;
            args.random_seed = 193;
            args.position = coords;
            args.goalpoint = coords;
            args.handle = no_handle;
            args.goal_handle = no_handle;
            bbCI_live_spawnEntity(&home.core.core, args, "SKELLY_LIVE", bbInstructionSource_internal, no_handle);
        } else {
            //bbDebug("this_player->state = %d\n", this_player->state);

            //bbVPMouseType_print(type)
            if (type == VPMouseLeftDown || type == VPMouseLeftDrag) {

                bbCoreInput_sendAIGoalpoint(&home.core.core,
                                        this_player->selected_entities[0],
                                        coords,
                                        home.core.core.actual_time,
                                        bbInstructionSource_internal,
                                        no_handle);


            }
        }
    return bbSuccess;
}
bbFlag bbPlayer_ClickUnit_answeringMaths(bbPlayers* players, bbHandle entity_handle, U64 control_keys, bbVPMouseType type){

}
bbFlag bbPlayer_KeyPress_answeringMaths(bbPlayers* players, U64 key, U64 control_keys) {
    //bbDebug("Player clicked key %llu, control keys:\n %064" PRIb64 "\n",
    //key,control_keys);

    if (key == 0) {

        bbTextInput* text_input;

        bbHandle widget_handle;
        bbDictionary_lookup(home.UI.widgets.dict, "TEXT_INPUT", &widget_handle);
        bbWidget* widget;
        bbVPool_lookup(home.UI.widgets.pool,(void**)&widget,widget_handle);

        text_input = widget->extra_data;

        bbMutexLock(&text_input->mutex);

        I32 input_number = bbStr_toI32(text_input->raw_buffer);

        bbMutexUnlock(&text_input->mutex);
        bbTextInput_setStr(text_input,"");

        //bbDebug("input number = %d\n", input_number);
        bbActionRequest_answerQuestion(&home.core.core,
                                            home.ECS.players.this_player,
                                            collision++,
                                            home.core.core.actual_time,
                                            home.core.core.actual_time,
                                            home.ECS.players.this_player,
                                            0,
                                      0,
                                      input_number);

        bbHandle target_handle, target_server_handle;
        I32 this_player = home.ECS.players.this_player;
        target_handle = home.ECS.players.players[this_player].target_entity;


        bbEntity_print(target_handle);

        bbFlag flag = bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS,target_handle,bbECS_ServerEntities,&target_server_handle,NULL);

        if (flag != bbSuccess) {
            bbFlag_print(flag)
            bbDebug("Server handle not found\n");
            return bbSuccess;
        }

        bbActionRequest_setPlayerAttack(&home.core.core,
                                            home.ECS.players.this_player,
                                            collision++,
                                            home.core.core.actual_time,
                                            home.core.core.actual_time,
                                            home.ECS.players.this_player,
                                            target_server_handle);
    }

    return bbSuccess;
}