#include "engine/logic/bbFlag.h"
#include "engine/player/bbPlayers.h"

#include "engine/ECS/bbECS.h"

bbFlag bbPlayers_init(bbCore* core, bbPlayers* players) {
    players->this_player = 0;
    for (int i = 0; i < 8; i++) {
        players->players[i].selected_entity = no_handle;
    }
    return bbSuccess;
}
