
#ifndef PLAYER_GOALPOINT_H
#define PLAYER_GOALPOINT_H

#include "engine/logic/bbHandle.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInbox.h"
#include "engine/core/bbInstruction.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbFlag.h"
#include "engine/network/bbNetwork.h"


bbFlag bbCoreInput_testClick4(bbCore* core,
                              bbMapCoords MC,
                              U64 time,
                              bbInstruction_source source,
                              bbHandle action);

bbFlag bbInstruction_testClick4_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbNetworkApp_setClick4(bbNetwork* network, bbMapCoords MC, U64 time, U32 collision);


#endif //PLAYER_GOALPOINT_H