#ifndef BB_PLAYERS
#define BB_PLAYERS

#define MAX_SELECTED_ENTITIES 32
#define num_players 32
#include "engine/ECS/bbECS.h"

typedef struct {
    bbComponent component;
    I32 player;
} bbPlayerComponent;

typedef struct {
    bbHandle selected_entities[MAX_SELECTED_ENTITIES];
} bbPlayer;

typedef struct {
    bbSystem system;
    I32 this_player;

    bbPlayer players[num_players];

} bbPlayers;


bbFlag bbPlayers_init(bbPlayers* system, bbECS* ECS);

bbFlag bbAction_setPlayerEntity(void* Core,
                       U32 sender,
                       U32 collision,
                       U64 created_tick,
                       U64 act_tick,
                       U32 player,
                       bbHandle server_handle);


//bbFlag bbCoreInbox_SetPlayerEntity(bbCore* core, U32 player, bbHandle server_handle);
//bbFlag bbCoreInbox_setPlayerEntity_fn(bbCore* core, bbCoreInboxMessage* message);
bbFlag bbCI_setPlayerEntity(bbCore* core, I32 player_index, bbHandle entity_handle,  bbInstruction_source source, bbHandle action);
bbFlag bbI_unsetPlayerEntity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_setPlayerEntity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbAction_setPlayerEntity_fn(bbCore* core, bbAction* action);
#endif //BB_PLAYERS