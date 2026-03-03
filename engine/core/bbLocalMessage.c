#include "engine/core/bbLocalMessage.h"

#include "engine/core/bbAction.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/threadsafe/bbThreadedQueue.h"

bbFlag bbLocalMessage_setString_fn(bbCore* core, bbLocalMessage* message)
{
    bbCoreInput_setString(core, message->data.string, bbInstructionSource_input, no_handle);
    //undo message?

    return bbSuccess;
}

bbFlag bbLocalMessage_unfreezeButton_fn(bbCore* core, bbLocalMessage* message)
{
    bbCoreInput_unfreezeButton(core, message->data.string, bbInstructionSource_input, no_handle);
    //undo message?

    return bbSuccess;
}

bbFlag bbLocalMessage_actionUnfreeze_fn(bbCore* core, bbLocalMessage* message)
{

    bbDebug("time = %lu\n", core->simulation_time  + 60);
    bbAction_unfreezeButton(core,
                           0,
                           rand(),
                           0,
                           core->simulation_time + 60,
                           message->data.string);

    return bbSuccess;
}

bbFlag bbCore_checkLocalMessages(bbCore* core)
{
    bbLocalMessage* message;

    bbFlag flag;

    while (1)
    {
        flag = bbThreadedQueue_popR(&core->local_message_queue,(void**)&message);
        if (flag != bbSuccess) return bbSuccess;


        switch (message->type)
        {
        case bbLocalMessage_setString:
            bbLocalMessage_setString_fn(core, message);
            bbCore_react(core);
            break;


        case bbLocalMessage_unfreezeButton:
            bbLocalMessage_unfreezeButton_fn(core, message);
            bbCore_react(core);
            break;

        case bbLocalMessage_actionUnfreeze:
            bbLocalMessage_actionUnfreeze_fn(core, message);
            bbCore_react(core);
            break;
        default:

            bbDebug("Unknown local message type");
        }
    }
}