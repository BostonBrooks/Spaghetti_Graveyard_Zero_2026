#ifndef BB_PLAYERS
#define BB_PLAYERS

#define MAX_SELECTED_ENTITIES 32
#define NUM_PLAYERS 32
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

    bbPlayer players[NUM_PLAYERS];

} bbPlayers;


bbFlag bbPlayers_init(bbPlayers* system, bbECS* ECS);

///On clicking player character, request change of player character from code
bbFlag bbCoreInbox_SetPlayerEntity(bbCore* core, U32 player, bbHandle server_handle);

///Core responds to mouse click
bbFlag bbCoreInbox_setPlayerEntity_fn(bbCore* core, bbCoreInboxMessage* message);

///Request player character change from server
bbFlag bbAction_setPlayerEntity(void* Core,
                       U32 sender,
                       U32 collision,
                       U64 created_tick,
                       U64 act_tick,
                       U32 player,
                       bbHandle server_handle);

///Server responds, switch player character
bbFlag bbAction_setPlayerEntity_fn(bbCore* core, bbAction* action);

///Notify core of character change
bbFlag bbCI_setPlayerEntity(bbCore* core, I32 player_index, bbHandle entity_handle,  bbInstruction_source source, bbHandle action);

///Modify player entity
bbFlag bbI_setPlayerEntity_fn(bbCore* core, bbInstruction* instruction);

///Rollback modification
bbFlag bbI_unsetPlayerEntity_fn(bbCore* core, bbInstruction* instruction);
#endif //BB_PLAYERS