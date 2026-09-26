
#include "engine/ECS/players/bbPlayers.h"
#include "engine/logic/bbFlag.h"

bbFlag bbPlayer_ClickMap_null(bbPlayers* players, bbMapCoords coords, U64 control_keys) ;
bbFlag bbPlayer_ClickUnit_null(bbPlayers* players, bbHandle entity_handle, U64 control_keys) ;
bbFlag bbPlayer_KeyPress_null(bbPlayers* players, U64 key, U64 control_keys);


bbFlag bbPlayers_populate(bbPlayers* system) {
    system->states[bbPlayer_stateNULL].click_map = bbPlayer_ClickMap_null;
    system->states[bbPlayer_stateNULL].click_unit = bbPlayer_ClickUnit_null;
    system->states[bbPlayer_stateNULL].key_press = bbPlayer_KeyPress_null;

    bbStr_setStr(
        system->states[bbPlayer_stateNULL].bbInputMode_key,
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