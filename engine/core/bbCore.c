#include <stdlib.h>
#include <stddef.h>
#include "engine/core/bbCore.h"

#include "bbInstruction.h"
#include "engine/core/bbLocalMessage.h"
#include "engine/core/bbInstruction.h"
#include "engine/core/bbAction.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/threadsafe/bbThreadedPool.h"

bbFlag bbCore_init(bbCore* core)
{
    bbVPool_newBloated(&core->instruction_pool, sizeof(bbInstruction), 100, 1000);
    bbList_init(&core->do_stack, core->instruction_pool, NULL, offsetof(bbInstruction, list_element),NULL);
    bbList_init(&core->undo_stack, core->instruction_pool, NULL, offsetof(bbInstruction, list_element),NULL);

    bbVPool_newThreaded(&core->local_message_pool, sizeof(bbLocalMessage),1000);

    bbThreadedQueue_init(&core->local_message_queue,
                          core->local_message_pool,
                          sizeof(bbLocalMessage),
                          1000,offsetof(bbLocalMessage, list_element));

    bbVPool_newBloated(&core->action_pool,sizeof(bbAction),100,1000);
    bbList_init(&core->action_queue, core->action_pool, NULL, offsetof(bbAction, header.list_element),bbAction_compare);

    core->simulation_time = 0;
 return bbSuccess;
}

bbFlag bbCore_react(bbCore* core)
{
    bbFlag flag;
    bbInstruction* instruction;

    while (1)
    {
        flag = bbList_popL(&core->do_stack, (void**)&instruction);
        if (flag != bbSuccess) return bbSuccess;

        switch (instruction->type)
        {

///(2) core reacts to instruction

        case bbInstruction_setTime:
            bbInstruction_setTime_fn(core, instruction);
            break;

        case bbInstruction_setString:
            bbInstruction_setString_fn(core, instruction);
            break;

        case bbInstruction_unfreezeButton:
            bbInstruction_unfreezeButton_fn(core, instruction);
            break;

        case bbInstruction_checkActions:
            bbInstruction_checkActions_fn(core, instruction);
            break;

        case bbInstruction_netsendButton:
            bbInstruction_netsendButton_fn(core, instruction);
            break;
        case bbInstruction_netcodeButton:

            bbInstruction_netcodeButton_fn(core, instruction);
            break;

        case bbInstruction_loopAction:

            bbInstruction_loopAction_fn(core, instruction);
            break;


        case bbInstruction_netpauseButton:

            bbInstruction_netpauseButton_fn(core, instruction);
            break;

        case bbInstruction_updateBall:
            bbInstruction_updateBall_fn(core, instruction);
            break;

        case bbInstruction_updatePaddle:
            bbInstruction_updatePaddle_fn(core, instruction);
            break;

        case bbInstruction_keyDown:
            bbInstruction_keyDown_fn(core, instruction);
            break;

        case bbInstruction_keyUp:
            bbInstruction_keyUp_fn(core, instruction);
            break;


        case bbInstruction_setPaddleDirection:
            bbInstruction_setPaddleDirection_fn(core, instruction);
            break;

        case bbInstruction_setPaddleVelocity:
            bbInstruction_setPaddleVelocity_fn(core, instruction);
            break;
        default:
            bbDebug("Unknown instruction type: %d\n", instruction->type);
        }

    }

    return bbSuccess;
}

bbFlag bbCore_rewind(bbCore* core)
{
    bbFlag flag;
    bbInstruction* instruction;

    while (1)
    {
        flag = bbList_popL(&core->undo_stack, (void**)&instruction);
        if (flag != bbSuccess) return bbSuccess;

        switch (instruction->type)
        {
        case bbInstruction_unsetTime:
            bbInstruction_unsetTime_fn(core, instruction);
            break;

            ///(6) core "un-reacts" to instruction
        case bbInstruction_unsetString:
            bbInstruction_unsetString_fn(core, instruction);
            break;

        case bbInstruction_uncheckActions:
            bbInstruction_uncheckActions_fn(core, instruction);
            break;

        case bbInstruction_unupdateBall:
            bbInstruction_unupdateBall_fn(core, instruction);
            break;

        case bbInstruction_unupdatePaddle:
            bbInstruction_unupdatePaddle_fn(core, instruction);
            break;


        default:
            bbDebug("Unknown undo instruction type");
        }

    }
    return bbSuccess;
}

bbFlag bbCore_rewindUntil(bbCore* core, U64 time)
{
    bbFlag flag;
    bbInstruction* instruction;

    while (core->simulation_time > time)
    {
        flag = bbList_popL(&core->undo_stack, (void**)&instruction);
        if (flag != bbSuccess) return bbSuccess;

        switch (instruction->type)
        {
        case bbInstruction_unsetTime:
            bbInstruction_unsetTime_fn(core, instruction);
            break;

            ///(6) core "un-reacts" to instruction
        case bbInstruction_unsetString:
            bbInstruction_unsetString_fn(core, instruction);
            break;

        case bbInstruction_uncheckActions:
            bbInstruction_uncheckActions_fn(core, instruction);
            break;


        default:
            bbDebug("Unknown undo instruction type\n");
        }

    }
    return bbSuccess;
}

