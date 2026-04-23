
#include "engine/core/bbAction.h"
#include "engine/core/bbCore.h"
#include "engine/logic/bbFlag.h"
#include "engine/pong/bbBall.h"
#include "engine/core/bbInstruction.h"
#include "engine/data/bbHome.h"
#include "engine/pong/bbPaddle.h"

bbFlag bbVInstruction_updateBall_fn(bbCore* core, bbInstruction* instruction)
{
    bbBall* ball = instruction->data.three_handles.handle1.ptr;
    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unupdateBall;
    //undo_instruction->data.unsigned_long = core->simulation_time;

    undo_instruction->data.three_handles.handle1.i32x2.x = ball->position.x;
    undo_instruction->data.three_handles.handle1.i32x2.y = ball->position.y;
    undo_instruction->data.three_handles.handle2.i32x2.x = ball->velocity.x;
    undo_instruction->data.three_handles.handle2.i32x2.y = ball->velocity.y;

    undo_instruction->data.three_handles.handle3.ptr = ball;
    undo_instruction->source = instruction->source;

    bbBall_Update(ball);



    //core->simulation_time = instruction->data.unsigned_long ;

    //printf("+time = %lu\n", core->simulation_time);
    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()


}

bbFlag bbVInstruction_unupdateBall_fn(bbCore* core, bbInstruction* instruction)
{
    //core->simulation_time = instruction->data.unsigned_long ;

    bbBall* ball = instruction->data.three_handles.handle3.ptr;


    ball->position.x = instruction->data.three_handles.handle1.i32x2.x;
    ball->position.y = instruction->data.three_handles.handle1.i32x2.y;
    ball->velocity.x = instruction->data.three_handles.handle2.i32x2.x;
    ball->velocity.y = instruction->data.three_handles.handle2.i32x2.y;

    //printf("-time = %lu\n", core->simulation_time);
    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        //TODO place instruction->redo_instruction into core->action_queue
        bbAction* redo_action;
        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    bbNotHere()

}
bbFlag bbVInstruction_updatePaddle_fn(bbCore* core, bbInstruction* instruction)
{
    bbPaddle* paddle = instruction->data.three_handles.handle1.ptr;
    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unupdatePaddle;
    //undo_instruction->data.unsigned_long = core->simulation_time;

    undo_instruction->data.three_handles.handle1.i32x2.x = paddle->position.x;
    undo_instruction->data.three_handles.handle1.i32x2.y = paddle->position.y;
    undo_instruction->data.three_handles.handle2.i32x2.x = paddle->velocity.x;
    undo_instruction->data.three_handles.handle2.i32x2.y = paddle->velocity.y;

    undo_instruction->data.three_handles.handle3.ptr = paddle;
    undo_instruction->source = instruction->source;

    bbPaddle_Update(paddle);



    //core->simulation_time = instruction->data.unsigned_long ;

    //printf("+time = %lu\n", core->simulation_time);
    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()


}

bbFlag bbVInstruction_unupdatePaddle_fn(bbCore* core, bbInstruction* instruction)
{
    //core->simulation_time = instruction->data.unsigned_long ;

    bbPaddle* paddle = instruction->data.three_handles.handle3.ptr;


    paddle->position.x = instruction->data.three_handles.handle1.i32x2.x;
    paddle->position.y = instruction->data.three_handles.handle1.i32x2.y;
    paddle->velocity.x = instruction->data.three_handles.handle2.i32x2.x;
    paddle->velocity.y = instruction->data.three_handles.handle2.i32x2.y;

    //printf("-time = %lu\n", core->simulation_time);
    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        //TODO place instruction->redo_instruction into core->action_queue
        bbAction* redo_action;
        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    bbNotHere()

}

bbFlag bbVInstruction_setPaddleDirection_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->data.three_handles.handle1.u64 == left_up)

    {

        bbPaddle* paddle = &home.core.paddle1;
        paddle->velocity.y = - 7*SCREEN_PPP;
    }
    if (instruction->data.three_handles.handle1.u64 == left_stop)

    {

        bbPaddle* paddle = &home.core.paddle1;
        paddle->velocity.y = - 0*SCREEN_PPP;
    }
    if (instruction->data.three_handles.handle1.u64 == left_down)

    {

        bbPaddle* paddle = &home.core.paddle1;
        paddle->velocity.y =  7*SCREEN_PPP;
    }
    return bbSuccess;
}

bbFlag bbVInstruction_setPaddleVelocity_fn(bbCore* core, bbInstruction* instruction)
{
    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unsetPaddleVelocity;
    undo_instruction->data.three_handles.handle1.i32x2.x =
        instruction->data.three_handles.handle1.i32x2.x;
    undo_instruction->source = instruction->source;

    if (instruction->data.three_handles.handle1.i32x2.x == 0)
    {
        bbPaddle* paddle = &home.core.paddle1;
        undo_instruction->data.three_handles.handle1.i32x2.y = paddle->velocity.y;
        paddle->velocity.y = instruction->data.three_handles.handle1.i32x2.y;
    } else {
        bbPaddle* paddle = &home.core.paddle2;
        undo_instruction->data.three_handles.handle1.i32x2.y = paddle->velocity.y;
        paddle->velocity.y = instruction->data.three_handles.handle1.i32x2.y;
    }

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()
}


bbFlag bbVInstruction_unsetPaddleVelocity_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->data.three_handles.handle1.i32x2.x == 0)
    {
        bbPaddle* paddle = &home.core.paddle1;
        paddle->velocity.y = instruction->data.three_handles.handle1.i32x2.y;
    } else {
        bbPaddle* paddle = &home.core.paddle2;
        paddle->velocity.y = instruction->data.three_handles.handle1.i32x2.y;
    }

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }


    bbNotHere()
}
