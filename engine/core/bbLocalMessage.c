#include "engine/core/bbLocalMessage.h"

#include "bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/userinterface/bbUI_Inbox.h"


bbFlag bbLocalMessage_printString_fn(bbCore* core, bbLocalMessage* message)
{
    bbCoreInput_printString(core, message->data.string.string, true);
    //undo message?

    return bbSuccess;
}

bbFlag bbLocalMessage_unfreezeButton_fn(bbCore* core, bbLocalMessage* message)
{
    bbCoreInput_unfreezeButton(core, message->data.string.string, true);
    //undo message?

    return bbSuccess;
}

bbFlag bbLocalMessage_unfreezeButton2_fn(bbCore* core, bbLocalMessage* message)
{
    bbCoreInput_unfreezeButton2(core, message->data.string.string, true);
    //undo message?

    return bbSuccess;
}

bbFlag bbLocalMessage_netsendButton_fn(bbCore* core, bbLocalMessage* message)
{
    bbCoreInput_netsendButton(core,message->data.string.string);

    return bbSuccess;
}

bbFlag bbLocalMessage_actionUnfreeze_fn(bbCore* core, bbLocalMessage* message)
{

    bbDebug("time = %lu\n", core->simulation_time  + 60);
    bbAction_unfreezeButton(core,
                           0,
                           0,
                           0,
                           core->simulation_time + 60,
                           message->data.string.string);

    return bbSuccess;
}

bbFlag bbLocalMessage_netcodeButton_fn(bbCore* core, bbLocalMessage* message)
{

    bbDebug("Send message to server\n");

    bbHandle handle = {0};
    bbCoreInput_netcodeButton(core,message->data.string.string, message->act_time,bbInstructionSource_input,handle);

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
        case bbLocalMessage_printString:
            bbLocalMessage_printString_fn(core, message);
            bbCore_react(core);
            break;

        case bbLocalMessage_unfreezeButton:
            bbLocalMessage_unfreezeButton_fn(core, message);
            bbCore_react(core);
            break;


        case bbLocalMessage_unfreezeButton2:
            bbLocalMessage_unfreezeButton2_fn(core, message);
            bbCore_react(core);
            break;


        case bbLocalMessage_netsendButton:
            bbLocalMessage_netsendButton_fn(core, message);
            bbCore_react(core);
            break;

        case bbLocalMessage_actionUnfreeze:
            bbLocalMessage_actionUnfreeze_fn(core, message);
            bbCore_react(core);
            break;



        case bbLocalMessage_netcodeButton:
            bbLocalMessage_netcodeButton_fn(core, message);
            bbCore_react(core);
            break;
            default:

            bbDebug("Unknown local message type");

        }
    }
    return bbNone;
}