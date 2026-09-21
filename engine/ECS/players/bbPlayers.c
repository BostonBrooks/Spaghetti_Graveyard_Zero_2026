#include "engine/ECS/players/bbPlayers.h"
#include "engine/core/bbCore.h"
#include "engine/ECS/bbECS.h"
#include "engine/logic/bbTerminal.h"


bbFlag bbPlayers_init(bbPlayers* players, bbECS* ECS){




    players->system.getComponent = NULL;
    players->system.getHandle = NULL;
    players->system.delete = NULL;
    players->system.ECS = ECS;

    ECS->systems[bbECS_Players] = (bbSystem* )players;

    return bbSuccess;
}
