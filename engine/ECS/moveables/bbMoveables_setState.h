#ifndef BB_MOVEABLES_SETSTATE_H
#define BB_MOVEABLES_SETSTATE_H

#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

bbFlag bbCI_Moveable_setGoalpoint(bbCore* core,
                             bbHandle moveable_handle,
                             bbMapCoords goalpoint,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbCI_Moveable_setGoalMovable(bbCore* core,
                             bbHandle moveable_handle,
                             bbHandle goal_moveable_handle,
                             bbInstruction_source source,
                             bbHandle action);


bbFlag bbCI_Moveable_setIdle(bbCore* core,
                             bbHandle moveable_handle,
                             bbHandle goal_moveable_handle,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbI_Moveable_setState_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_Moveable_unsetState_fn(bbCore* core, bbInstruction* instruction);



bbFlag bbCS_Moveable_setGoalpoint(bbCore* core,
                             bbHandle moveable_handle,
                             bbMapCoords goalpoint,
                             bbInstruction_source source,
                             bbHandle action);

#endif //BB_MOVEABLES_SETSTATE_H