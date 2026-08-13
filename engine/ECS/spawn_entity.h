
#ifndef SPAWN_UNIT_H
#define SPAWN_UNIT_H

#include "engine/logic/bbHandle.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInbox.h"
#include "engine/core/bbInstruction.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbFlag.h"
#include "engine/network/bbNetwork.h"

bbFlag bbCoreInbox_TestClick2(bbCore* core, bbMapCoords mapCoords);
bbFlag bbCoreInbox_testClick2_fn(bbCore* core, bbCoreInboxMessage* message);

bbFlag bbCoreInput_testClick3(bbCore* core,
                                  bbMapCoords MC,
                                  U64 time,
                                  bbInstruction_source source,
                                  bbHandle action);

bbFlag bbInstruction_testClick3_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbNetworkApp_setClick3(bbNetwork* network, bbMapCoords MC, U64 time, U32 collision);


bbFlag bbAction_spawnEntity(void* Core,
                            bbMapCoords map_coords,
                            bbHandle server_entity,
                            U32 collision,
                            U64 act_tick);

bbFlag bbCoreInput_spawnTestEntity2(bbCore* core,
                                   bbMapCoords MC,
                                   bbHandle server_entity,
                                   bbInstruction_source source,
                                   bbHandle action);

bbFlag bbInstruction_spawnTestEntity2_fn(bbCore* core, bbInstruction* instruction);

#endif //SPAWN_UNIT_H