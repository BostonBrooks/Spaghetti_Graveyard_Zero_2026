#include "engine/ECS/players/bbPlayers.h"
#include "engine/core/bbCore.h"
#include "engine/ECS/bbECS.h"
#include "engine/logic/bbTerminal.h"


bbFlag bbPlayers_init(bbPlayers* players, bbECS* ECS){

    players->this_player = -1;

    for (I32 i = 0; i < num_players; i++) {
        for (I32 j = 0; j < num_selected_entities; j++) {
            players->players[i].selected_entities[j] = players->system.pool->null;
        }
    }

    players->system.getComponent = NULL;
    players->system.getHandle = NULL;
    players->system.delete = NULL;
    players->system.ECS = ECS;

    ECS->systems[bbECS_Players] = (bbSystem* )players;

    return bbSuccess;
}
