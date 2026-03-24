#include "bbCore.h"
#include "bbInstruction.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbString.h"
#include "engine/pong/bbBall.h"
#include "engine/pong/bbPaddle.h"

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

bbFlag bbCoreInput_updateBall(bbCore* core, bbBall* ball, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->data.three_handles.handle1.ptr = ball;
    instruction->type = bbInstruction_updateBall;
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
    instruction->type = bbInstruction_updatePaddle;
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
    instruction->type = bbInstruction_setPaddleDirection;
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
    instruction->type = bbInstruction_setPaddleVelocity;
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

bbFlag bbCoreInput_netsendButton(bbCore* core, char* string)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->type = bbInstruction_netsendButton;
    bbStr_setStr(instruction->data.string, string, KEY_LENGTH);

    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;
}



bbFlag bbCoreInput_netpauseButton(bbCore* core, char* string)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->type = bbInstruction_netpauseButton;
    bbStr_setStr(instruction->data.string, string, KEY_LENGTH);

    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;
}

bbFlag bbCoreInput_keyUp(bbCore* core, I32 key_code, U64 time,
                                  bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->type = bbInstruction_keyUp;
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
    instruction->type = bbInstruction_keyDown;
    instruction->data.three_handles.handle1.u64 = key_code;
    instruction->act_time = time;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

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