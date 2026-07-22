#include "engine/logic/bbHandle.h"
bbFlag bbCoreInbox_TestClick(bbCore* core, bbMapCoords mapCoords);
bbFlag bbCoreInbox_testClick_fn(bbCore* core, bbLocalMessage* message);

bbFlag bbCoreInput_spawnEntityOut(bbCore* core,
                                  bbMapCoords MC,
                                  U64 time,
                                  bbInstruction_source source,
                                  bbHandle action);

bbFlag bbInstruction_spawnEntityOut_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbNetworkApp_spawnEntityOut(bbNetwork* network, bbMapCoords MC, U64 time, U32 collision);


bbFlag bbAction_spawnUnit(void* Core,
                            bbMapCoords map_coords,
                            bbHandle server_entity,
                            U32 collision,
                            U64 act_tick);

bbFlag bbCoreInput_spawnTestEntity(bbCore* core,
                                   bbMapCoords MC,
                                   bbHandle server_entity,
                                   bbInstruction_source source,
                                   bbHandle action);

bbFlag bbInstruction_spawnTestEntity_fn(bbCore* core, bbInstruction* instruction);
