#ifndef BB_TUX_H

#include "engine/logic/bbFlag.h"

//The player clicks the viewport which calls Viewport_LeftDown, this calls
bbFlag bbLocalMessage_SpawnEntity(bbEntityType type, bbMapCoords MC);

//This places a message on core->local_message_queue with
//message->type = bbLocalMessage_spawnEntity

//bbCore_checkLocalMessages() checks messages and calls 
bbFlag bbLocalMessage_spawnEntityOut_fn(bbCore* core, bbLocalMessage* message);

//This calls
bbFlag bbCoreInput_spawnEntityOut(...);

//This places an instruction on the core with instruction->type = bbInstruction_spawnEntityOut;

bbFlag bbCore_react(bbCore* core)
//This calls
bbFlag bbInstruction_spawnEntityOut_fn(bbCore* core, bbInstruction* instruction);

//then
bbFlag bbNetworkApp_spawnEntityOut(bbNetwork* network, bbMapCoords MC, U64 time, U32 collision);

//message is sent to the server and back

bbFlag bbNetworkApp_checkInbox(bbNetwork* network);

//calls

bbFlag bbAction_spawnEntity(void* Core,
                            bbMapCoords map_coords,
                            I32 entity_index,
                            I32 moveable_index,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            U8 player);

//This puts a message on the action queue with action->header.type = bbActionType_spawnEntity;

bbFlag bbInstruction_checkActions_fn(bbCore* core, bbInstruction* instruction);

//calls

//bbCoreInput_spawnEntityIn();

//this pushes and instruction on core->do_stack with
//instruction->type = bbInstruction_spawnEntityIn

//bbFlag bbCore_react(bbCore* core) calls bbInstruction_spawnEntityIn_fn

// bbInstruction_spawnEntityIn_fn sets up an undo instruction
//then calls bbEntity_NewEntity

//this calls bbAgent_newEntity
//and then calls bbUI_Inbox_NewBanana()


