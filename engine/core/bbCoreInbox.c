#include "engine/core/bbCoreInbox.h"
#ifdef DEFINE_SKELLYCHASE
#include "core/core_inputs.h"
#endif
#include "engine/core/bbAction.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/spawn_entity.h"
#include "engine/threadsafe/bbThreadedQueue.h"

bbFlag bbCoreInbox_setString_fn(bbCore* core, bbCoreInboxMessage* message)
{
    bbCoreInput_setString(core, message->data.key, bbInstructionSource_input, no_handle);
    //undo message?

    return bbSuccess;
}

bbFlag bbCoreInbox_unfreezeButton_fn(bbCore* core, bbCoreInboxMessage* message)
{
    bbCoreInput_unfreezeButton(core, message->data.key, bbInstructionSource_input, no_handle);


    return bbSuccess;
}


bbFlag bbCoreInbox_testClick_fn(bbCore* core, bbCoreInboxMessage* message)
{

    bbCoreInput_testClick(core, message->data.map_coords, bbInstructionSource_input, no_handle);


    return bbSuccess;
}

bbFlag bbCore_checkInbox(bbCore* core)
{
    bbCoreInboxMessage* message;

    bbFlag flag;

    while (1)
    {
        flag = bbThreadedQueue_popR(&core->local_message_queue,(void**)&message);
        if (flag != bbSuccess) return bbSuccess;


        switch (message->type)
        {
        case bbCoreInbox_setString:
            bbCoreInbox_setString_fn(core, message);
            bbCore_react(core);
            break;

        case bbCoreInbox_unfreezeButton:
            bbCoreInbox_unfreezeButton_fn(core, message);
            bbCore_react(core);
            break;

            case bbCoreInbox_testClick:
            bbCoreInbox_testClick2_fn(core, message);
            bbCore_react(core);
            break;

        default:

            bbDebug("Unknown local message type\n");
        }
    }
}

