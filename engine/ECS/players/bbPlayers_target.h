#ifndef BBPLAYERS_TARGET
#define BBPLAYERS_TARGET
#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbFlag.h"
bbFlag bbActionRequest_setPlayerTarget(void* Core,
                                       U32 sender,
                                       U32 collision,
                                       bbTime created_tick,
                                       bbTime act_tick,
                                       U32 player,
                                       bbHandle target_server_handle);

bbFlag bbAction_setPlayerTarget_fn(bbCore* core, bbAction* action);

bbFlag bbActionRequest_setPlayerAttack(void* Core,
                       U32 sender,
                       U32 collision,
                       bbTime created_tick,
                       bbTime act_tick,
                       U32 player,
                       bbHandle target_server_handle);

bbFlag bbAction_setPlayerAttack_fn(bbCore* core, bbAction* action);


bbFlag bbCI_setPlayerTarget(bbCore* core, U32 player, bbHandle entity_handle, bbInstruction_source source, bbHandle action);
bbFlag bbCS_setPlayerTarget(bbCore* core, U32 player, bbHandle entity_handle, bbInstruction_source source, bbHandle action);

bbFlag bbI_setPlayerTarget_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unsetPlayerTarget_fn(bbCore* core, bbInstruction* instruction);


#endif //BBPLAYERS_TARGET