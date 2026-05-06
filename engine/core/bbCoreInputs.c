#include "bbCore.h"
#include "bbInstruction.h"

#ifdef DEFINE_PONG
#include "core/instructions.h"
#endif

#ifdef DEFINE_SKELLYCHASE
#include "core/instructions.h"
#endif


#include "engine/logic/bbFlag.h"
#include "engine/logic/bbString.h"
#include "games/game0/maps/pong/code/bbBall.h"
#include "games/game0/maps/pong/code/bbPaddle.h"

bbFlag bbCoreInput_setString(bbCore* core, char* string, bbInstruction_source source, bbHandle action)
{

    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbInstruction_setString;
    bbStr_setStr(instruction->data.string, string, KEY_LENGTH);
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

bbFlag bbCoreInput_unfreezeButton(bbCore* core, char* string, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbInstruction_unfreezeButton;
    bbStr_setStr(instruction->data.string, string, KEY_LENGTH);
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

bbFlag bbCoreInput_checkActions(bbCore* core, U64 time, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbInstruction_checkActions;
    instruction->source = source;
    instruction->redo_instruction = action;
    instruction->data.unsigned_long = time;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}


bbFlag bbCoreInput_setTime(bbCore* core, U64 time, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbInstruction_setTime;
    instruction->data.unsigned_long = time;
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

#ifdef DEFINE_PONG
bbFlag bbCoreInput_updateBall(bbCore* core, bbBall* ball, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->data.three_handles.handle1.ptr = ball;
    instruction->type = bbVInstruction_updateBall;
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

bbFlag bbCoreInput_updatePaddle(bbCore* core, bbPaddle* paddle, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->data.three_handles.handle1.ptr = paddle;
    instruction->type = bbVInstruction_updatePaddle;
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}
bbFlag bbCoreInput_setPaddleDirection(bbCore* core, I32 direction, U64 time,
                                  bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->data.three_handles.handle1.u64 = direction;
    instruction->type = bbVInstruction_setPaddleDirection;
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

bbFlag bbCoreInput_setPaddleVelocity(bbCore* core, U8 player, I32 velocity, U64 time,
                                  bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->data.three_handles.handle1.i32x2.x = player;
    instruction->data.three_handles.handle1.i32x2.y = velocity;
    instruction->type = bbVInstruction_setPaddleVelocity;
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}
#endif //DEFINE_PONG
bbFlag bbCoreInput_netsendButton(bbCore* core, char* string)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->type = bbInstruction_netsendButton;
    bbStr_setStr(instruction->data.string, string, KEY_LENGTH);

    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;
}
/*
bbFlag bbCoreInput_approachGoalpoint(bbCore* core,bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->source = source;
    instruction->redo_instruction = action;
    instruction->type = bbInstruction_approachGoalpoint;

    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;
}*/


bbFlag bbCoreInput_netpauseButton(bbCore* core, char* string)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->type = bbInstruction_netpauseButton;
    bbStr_setStr(instruction->data.string, string, KEY_LENGTH);

    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;
}
#ifdef DEFINE_PONG
bbFlag bbCoreInput_keyUp(bbCore* core, I32 key_code, U64 time,
                                  bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->type = bbVInstruction_keyUp;
    instruction->data.three_handles.handle1.u64 = key_code;
    instruction->act_time = time;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}
bbFlag bbCoreInput_keyDown(bbCore* core, I32 key_code, U64 time,
                                  bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbVInstruction_keyDown;
    instruction->data.three_handles.handle1.u64 = key_code;
    instruction->act_time = time;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}
#endif //DEFINE_PONG
bbFlag bbCoreInput_netcodeButton(bbCore* core, char* string, U64 time,
                                  bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);


    instruction->type = bbInstruction_netcodeButton;
    bbStr_setStr(instruction->data.string, string, KEY_LENGTH);
    instruction->act_time = time;
    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;
}

bbFlag bbCoreInput_loop(bbCore* core, char* string, U64 time,
                                  bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbInstruction_loopAction;
    instruction->act_time = time;
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}
#ifdef DEFINE_SKELLYCHASE
/*
bbFlag bbCoreInput_setViewpointOut(bbCore* core, bbMapCoords MC, U64 time,
                                  bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbInstruction_setViewpointOut;
    instruction->data.map_coords = MC;
    instruction->act_time = time;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}
*/


bbFlag bbCoreInput_setGoalpointOut(bbCore* core, bbMapCoords MC, U64 time,
                                  bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbVInstruction_setGoalpointOut;
    instruction->data.map_coords = MC;
    instruction->act_time = time;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

/*bbFlag bbCoreInput_setViewpointIn(bbCore* core, bbMapCoords MC, U64 time,
                                  bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbInstruction_setViewpointIn;
    instruction->data.map_coords = MC;
    instruction->act_time = time;
    instruction->source = source;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}
*/

//bbCoreInput_setGoalpointIn(core, action->map_coords,action->header.act_tick, action->header.player,bbInstructionSource_action,handle);
bbFlag bbCoreInput_setGoalpointIn(bbCore* core, bbMapCoords MC, U64 time,U8 player,
                                  bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbVInstruction_setGoalpointIn;
    instruction->data.map_coords = MC;
    instruction->act_time = time;
    instruction->source = source;
    instruction->player = player;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}


bbFlag bbCoreInput_updateMoveables(bbCore* core,
                                  bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);
    instruction->type = bbVInstruction_updateMoveables;
    instruction->source = source;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

#endif