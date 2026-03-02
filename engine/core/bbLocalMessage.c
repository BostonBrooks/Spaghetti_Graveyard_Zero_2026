#include "engine/core/bbLocalMessage.h"

#include "bbCoreInputs.h"
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
        default:

            bbDebug("Unknown local message type");
        }
    }
}