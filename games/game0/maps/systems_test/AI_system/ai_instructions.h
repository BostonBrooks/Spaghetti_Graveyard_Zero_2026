#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

//Nowhere in bbCI_updateAI is bbCore_react() called until al AIs have been updated.
//Changes to state dont occur until when bbCore_react() is called
//bbI_unupdateAI_fn doesnt do much but the state changes are tracked by the above state change instructions

bbFlag bbCI_updateAI(bbCore* core, bbInstruction_source source, bbHandle action);
bbFlag bbCS_updateAI(bbCore* core, bbInstruction_source source, bbHandle action);
bbFlag bbI_updateAI_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unupdateAI_fn(bbCore* core, bbInstruction* instruction);