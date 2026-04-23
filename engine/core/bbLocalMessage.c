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


bbFlag bbLocalMessage_retroactionUnfreeze_fn(bbCore* core, bbLocalMessage* message)
{

    bbDebug("time = %lu\n", core->simulation_time  - 60);
    bbAction_unfreezeButton(core,
                           0,
                           rand(),
                           0,
                           core->simulation_time - 60,
                           message->data.string);

    return bbSuccess;
}

bbFlag bbLocalMessage_netsendButton_fn(bbCore* core, bbLocalMessage* message)
{
    bbCoreInput_netsendButton(core,message->data.string);

    return bbSuccess;
}

bbFlag bbLocalMessage_netcodeButton_fn(bbCore* core, bbLocalMessage* message)
{


    bbHandle handle = {0};
    bbCoreInput_netcodeButton(core,message->data.string, message->act_time,bbInstructionSource_input,handle);

    return bbSuccess;
}
bbFlag bbLocalMessage_netpauseButton_fn(bbCore* core, bbLocalMessage* message)
{
    bbCoreInput_netpauseButton(core,message->data.string);

    return bbSuccess;
}

bbFlag bbLocalMessage_actionLoop_fn(bbCore* core, bbLocalMessage* message)
{

    bbAction_loop(core,
                           0,
                           rand(),
                           0,
                           message->act_time,
                           message->data.string);

    return bbSuccess;
}
#ifdef DEFIBE_PONG
bbFlag bbLocalMessage_keyUp_fn(bbCore* core, bbLocalMessage* message)
{

    bbHandle handle = {0};
    bbCoreInput_keyUp(core,message->data.three_handles.handle1.u64, message->act_time,bbInstructionSource_input,handle);

    return bbSuccess;
}

bbFlag bbLocalMessage_keyDown_fn(bbCore* core, bbLocalMessage* message)
{
    bbHandle handle = {0};
    bbCoreInput_keyDown(core,message->data.three_handles.handle1.u64, message->act_time,bbInstructionSource_input,handle);

    return bbSuccess;
}
#endif //DEFINE_PONG
bbFlag bbLocalMessage_setViewpointOut_fn(bbCore* core, bbLocalMessage* message)
{
    bbHandle handle = {0};
    bbCoreInput_setViewpointOut(core,
        message->data.map_coords, message->act_time,bbInstructionSource_input,handle);

    return bbSuccess;
}

bbFlag bbLocalMessage_setGoalpointOut_fn(bbCore* core, bbLocalMessage* message)
{
    bbHandle handle = {0};
    bbCoreInput_setGoalpointOut(core,
        message->data.map_coords, message->act_time,bbInstructionSource_input,handle);

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

        case bbLocalMessage_retroactionUnfreeze:
            bbLocalMessage_retroactionUnfreeze_fn(core, message);
            bbCore_react(core);
            break;

        case bbLocalMessage_netsendButton:
            bbLocalMessage_netsendButton_fn(core, message);
            bbCore_react(core);

        case bbLocalMessage_netcodeButton:
            bbLocalMessage_netcodeButton_fn(core, message);
            bbCore_react(core);
            break;



        case bbLocalMessage_actionLoop:
            bbLocalMessage_actionLoop_fn(core, message);
            bbCore_react(core);
            break;



        case bbLocalMessage_netpauseButton:
            bbLocalMessage_netpauseButton_fn(core, message);
            bbCore_react(core);
            break;


#ifdef DEFINE_PONG
        case bbLocalMessage_keyUp:
            bbLocalMessage_keyUp_fn(core, message);
            bbCore_react(core);
            break;



        case bbLocalMessage_keyDown:
            bbLocalMessage_keyDown_fn(core, message);
            bbCore_react(core);
            break;

#endif //DEFINE_PONG
        case bbLocalMessage_setViewpoint:
            bbLocalMessage_setViewpointOut_fn(core, message);
            bbCore_react(core);
            break;
        case bbLocalMessage_setGoalpoint:
            bbLocalMessage_setGoalpointOut_fn(core, message);
            bbCore_react(core);
            break;

        default:

            bbDebug("Unknown local message type\n");
        }
    }
}