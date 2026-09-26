#ifndef BB_PLAYERS
#define BB_PLAYERS

#define MAX_SELECTED_ENTITIES 32
#define NUM_PLAYERS 32
#include "engine/ECS/bbECS.h"


typedef enum {
    bbPlayer_stateNULL,
    bbPlayer_stateDefault,
    bbPlayer_stateNum,
} bbPlayer_state;


typedef struct bbPlayers bbPlayers;

typedef struct {
    bbComponent component;
    I32 player;
} bbPlayerComponent;

typedef struct {
    bbHandle selected_entities[MAX_SELECTED_ENTITIES];
    bbPlayer_state state;
} bbPlayer;

typedef bbFlag bbPlayer_ClickMap_fn(bbPlayers* players, bbMapCoords coords, U64 control_keys);
typedef bbFlag bbPlayer_ClickUnit_fn(bbPlayers* players, bbHandle entity_handle, U64 control_keys);
typedef bbFlag bbPlayer_KeyPress_fn(bbPlayers* players, U64 key, U64 control_keys);


typedef struct {
    bbPlayer_ClickMap_fn* click_map;
    bbPlayer_ClickUnit_fn* click_unit;
    bbPlayer_KeyPress_fn* key_press;
    char bbInputMode_key[KEY_LENGTH];
} bbPlayerState;


typedef struct bbPlayers{
    bbSystem system;
    ///this is not shared state
    I32 this_player;
    bbPlayerState states[bbPlayer_stateNum];
    bbPlayer players[NUM_PLAYERS];

} bbPlayers;

/// initialise data structures
bbFlag bbPlayers_init(bbPlayers* system, bbECS* ECS);

///load game-specific data
bbFlag bbPlayers_populate(bbPlayers* system);

bbFlag bbPlayer_ClickMap(bbPlayers* players, bbMapCoords coords, U64 control_keys);
bbFlag bbPlayer_ClickUnit(bbPlayers* players, bbHandle entity_handle, U64 control_keys);
bbFlag bbPlayer_KeyPress(bbPlayers* players, U64 key, U64 control_keys);

bbFlag bbCoreInput_setPlayerState(bbCore* core, U32 player, U32 state);

///On clicking player character, request change of player character from code
bbFlag bbCoreInbox_SetPlayerEntity(bbCore* core, U32 player, bbHandle entity_handle);

///Core responds to mouse click
bbFlag bbCoreInbox_setPlayerEntity_fn(bbCore* core, bbCoreInboxMessage* message);

///Request player character change from server
bbFlag bbActionRequest_setPlayerEntity(void* Core,
                       U32 sender,
                       U32 collision,
                       bbTime created_tick,
                       bbTime act_tick,
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