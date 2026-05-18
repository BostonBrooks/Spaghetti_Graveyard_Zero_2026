#ifndef BB_CORE_INPUTS_H
#define BB_CORE_INPUTS_H
#include "engine/core/bbInstruction.h"
#include "engine/core/bbCore.h"
#include "games/game0/maps/pong/code/bbBall.h"
#include "games/game0/maps/pong/code/bbPaddle.h"
#include "engine/agents/bbAgentFunctions.h"


bbFlag bbCoreInput_setString(bbCore* core, char* string, bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_checkActions(bbCore* core, U64 time, bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_setTime(bbCore* core, U64 time, bbInstruction_source source, bbHandle action);
bbFlag bbCoreInput_unfreezeButton(bbCore* core, char* string, bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_netsendButton(bbCore* core,char* string);

bbFlag bbCoreInput_netcodeButton(bbCore* core, char* string, U64 time,
                                  bbInstruction_source source, bbHandle action);


bbFlag bbCoreInput_loop(bbCore* core, char* string, U64 time,
                                  bbInstruction_source source, bbHandle action);


bbFlag bbCoreInput_netpauseButton(bbCore* core,char* string);

bbFlag bbCoreInput_updateBall(bbCore* core, bbBall* ball, bbInstruction_source source, bbHandle action);
bbFlag bbCoreInput_updatePaddle(bbCore* core, bbPaddle* paddle, bbInstruction_source source, bbHandle action);


bbFlag bbCoreInput_keyUp(bbCore* core, I32 key_code, U64 time,
                                  bbInstruction_source source, bbHandle action);
bbFlag bbCoreInput_keyDown(bbCore* core, I32 key_code, U64 time,
                                  bbInstruction_source source, bbHandle action);
bbFlag bbCoreInput_setPaddleDirection(bbCore* core, I32 direction, U64 time,
                                  bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_setPaddleVelocity(bbCore* core, U8 player, I32 velocity, U64 time,
                                  bbInstruction_source source, bbHandle action);

///Send message to graphics thread.

bbFlag bbCoreInput_setViewpointOut(bbCore* core, bbMapCoords MC, U64 time,
                                  bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_setViewpointIn(bbCore* core, bbMapCoords MC, U64 time,
                                  bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_setGoalpointOut(bbCore* core, bbMapCoords MC, U64 time,
                                  bbInstruction_source source, bbHandle action);


bbFlag bbCoreInput_setGoalpointIn(bbCore* core, bbMapCoords MC, U64 time,U8 player,
                                  bbInstruction_source source, bbHandle action);


bbFlag bbCoreInput_approachGoalpoint(bbCore* core,bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_updateMoveables(bbCore* core,bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_setGoalMoveable(bbCore* core,U64 time, I32 moveable, I32 goal_moveable,
                                 bbInstruction_source source, bbHandle action);


bbFlag bbCoreInput_setMoveableIdle(bbCore* core,U64 time, I32 moveable, bbMapCoords MC,
                                 bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_setMoveableType(bbCore* core,U64 time, I32 moveable, bbAgentCommandData data,
                                 bbInstruction_source source, bbHandle action);
#endif //BB_CORE_INPUTS
