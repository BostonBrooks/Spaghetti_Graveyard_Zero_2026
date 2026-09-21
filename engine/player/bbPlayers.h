#ifndef BB_PLAYER_H
#define BB_PLAYER_H
#include "engine/core/bbCore.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

typedef struct {
    bbHandle selected_entity;
} bbPlayer;

typedef struct {
    I32 this_player;
    bbPlayer players[8];
} bbPlayers;

bbFlag bbPlayers_init(bbCore* core, bbPlayers* players);







#endif //BB_PLAYER_H