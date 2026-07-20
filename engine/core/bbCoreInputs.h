#ifndef BB_CORE_INPUTS_H
#define BB_CORE_INPUTS_H
#include "engine/core/bbInstruction.h"
#include "engine/core/bbCore.h"
#include "games/game0/maps/pong/code/bbBall.h"
#include "games/game0/maps/pong/code/bbPaddle.h"
//#include "engine/entities/bbAgentFunctions.h"


bbFlag bbCoreInput_setString(bbCore* core, char* string, bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_checkActions(bbCore* core, U64 time, bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_setTime(bbCore* core, U64 time, bbInstruction_source source, bbHandle action);

#endif