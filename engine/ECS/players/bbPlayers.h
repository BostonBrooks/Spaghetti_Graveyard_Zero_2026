#ifndef BB_PLAYERS
#define BB_PLAYERS

#define num_selected_entities 32
#define num_players 32
#include "engine/ECS/bbECS.h"

typedef struct {
    bbComponent component;
    I32 player;
} bbPlayerComponent;

typedef struct {
    bbHandle selected_entities[num_selected_entities];
} bbPlayer;

typedef struct {
    bbSystem system;
    I32 this_player;

    bbPlayer players[num_players];

} bbPlayers;


bbFlag bbPlayers_init(bbPlayers* players, bbECS* ECS);

#endif //BB_PLAYERS