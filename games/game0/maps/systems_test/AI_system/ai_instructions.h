#ifndef AI_INSTRUCTIONS_H
#define AI_INSTRUCTIONS_H

#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/ECS/ECS.h"
#include "engine/ECS/AI_system/bbAI_System.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

//Nowhere in bbCI_updateAI is bbCore_react() called until al AIs have been updated.
//Changes to state dont occur until when bbCore_react() is called
//bbI_unupdateAI_fn doesnt do much but the state changes are tracked by the above state change instructions

//enum bbInstruction_updateAI
//enum bbInstruction_unupdateAI

bbFlag bbCI_updateAI(bbCore* core, bbInstruction_source source, bbHandle action);
bbFlag bbCS_updateAI(bbCore* core, bbInstruction_source source, bbHandle action);
bbFlag bbI_updateAI_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unupdateAI_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbCS_spawnAIComponent2(bbCore* core,
                             bbECS* ECS,
                             bbHandle entity,
                             I32 update,
                             I32 command,
                             bbAI_Component** this,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbCI_spawnAIComponent2(bbCore* core,
                             bbHandle entity,
                             I32 update,
                             I32 command,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbI_spawnAIComponent2_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unspawnAIComponent2_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbI_AI_setState_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_AI_unsetState_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbCI_AI_setState(bbCore* core,
                             bbHandle AI_handle,
                             U64 current_time,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbCI_AI_setMoving(bbCore* core,
                             bbHandle AI_handle,
                             U64 current_time,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbCI_AI_setIdle(bbCore* core,
                             bbHandle AI_handle,
                             U64 current_time,
                             bbInstruction_source source,
                             bbHandle action);


bbFlag bbCI_AI_setApproaching(bbCore* core,
                             bbHandle AI_handle,
                             bbHandle target_handle,
                             U64 current_time,
                             bbInstruction_source source,
                             bbHandle action);


bbFlag bbCI_AI_setStriking(bbCore* core,
                             bbHandle AI_handle,
                             bbHandle target_handle,
                             U64 current_time,
                             bbInstruction_source source,
                             bbHandle action);


bbFlag bbCI_AI_setRecovering(bbCore* core,
                             bbHandle AI_handle,
                             U64 current_time,
                             bbInstruction_source source,
                             bbHandle action);




bbFlag bbCI_AI_setApproaching(bbCore* core,
                             bbHandle AI_handle,
                             bbHandle target_handle,
                             U64 current_time,
                             bbInstruction_source source,
                             bbHandle action);

#endif //AI_INSTRUCTIONS_H