#ifndef player_send_goalpoint_h
#define player_send_goalpoint_h

#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbHandle.h"
#include "engine/network/bbNetwork.h"

#include "engine/logic/bbFlag.h"

bbFlag bbCoreInput_sendAIGoalpoint(bbCore* core,
                                 bbHandle entity,
                                 bbMapCoords MC,
                                 U64 time,
                                 bbInstruction_source source,
                                 bbHandle action);

// enum bbI_AI_sendGoalpoint

bbFlag bbInstruction_sendAIGoalpoint_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbNetworkApp_sendAIGoalpoint(bbNetwork* Network, bbHandle server_entity, bbMapCoords MC, U64 time, U32 collision);
//PACKETTYPE_SENDGOALPOINT

bbFlag bbNetworkApp_sendAIGoalpoint(bbNetwork* Network, bbHandle server_entity, bbMapCoords MC, U64 time, U32 collision);
bbFlag bbAction_setAIGoalpoint_fn(bbCore* core, bbAction* action) ;


#endif //player_send_goalpoint_h