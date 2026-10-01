
#include "engine/logic/bbTerminal.h"

extern U32 collision;

bbFlag bbPlayer_ClickMap_requestingMaths(bbPlayers* players, bbMapCoords coords, U64 control_keys, bbVPMouseType type) {
    bbHere();
}
bbFlag bbPlayer_ClickUnit_requestingMaths(bbPlayers* players, bbHandle entity_handle, U64 control_keys, bbVPMouseType type){
    bbHere();
}
bbFlag bbPlayer_KeyPress_requestingMaths(bbPlayers* players, U64 key, U64 control_keys){
    {
        bbDebug("Player clicked key %llu, control keys:\n %064" PRIb64 "\n",
        key,control_keys);

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

            bbDebug("input number = %d\n", input_number);
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
}


bbFlag bbPlayer_ClickMap_answeringMaths(bbPlayers* players, bbMapCoords coords, U64 control_keys, bbVPMouseType type){
    bbHere();
}
bbFlag bbPlayer_ClickUnit_answeringMaths(bbPlayers* players, bbHandle entity_handle, U64 control_keys, bbVPMouseType type){
    bbHere();
}
bbFlag bbPlayer_KeyPress_answeringMaths(bbPlayers* players, U64 key, U64 control_keys) {
    bbDebug("Player clicked key %llu, control keys:\n %064" PRIb64 "\n",
    key,control_keys);

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

        bbDebug("input number = %d\n", input_number);
        bbActionRequest_answerQuestion(&home.core.core,
                                            home.ECS.players.this_player,
                                            collision++,
                                            home.core.core.actual_time,
                                            home.core.core.actual_time,
                                            home.ECS.players.this_player,
                                            0,
                                      0,
                                      input_number);
    }

    return bbSuccess;
}