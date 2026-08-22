#include "engine/core/bbCoreInbox.h"

#include "games/game0/maps/systems_test/entity_spawner/live_spawn.h"

#include "engine/core/bbAction.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "games/game0/maps/systems_test/core/spawn_entity.h"
#include "engine/threadsafe/bbThreadedQueue.h"

bbFlag bbCoreInbox_setString_fn(bbCore* core, bbCoreInboxMessage* message)
{
    bbCoreInput_setString(core, message->data.key, bbInstructionSource_input, no_handle);
    //undo message?

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

        if (message->type >= bbCoreInbox_numTypes)
        {//bbDebug("inbox instruction = %d\n", message->type);
            bbCoreInbox_fn* inbox_fn = core->inbox_functions[message->type-bbCoreInbox_numTypes];

            inbox_fn(core, message);
            bbCore_react(core);
        } else
        {
            switch (message->type)
            {
            case bbCoreInbox_setString:
                bbCoreInbox_setString_fn(core, message);
                bbCore_react(core);
                break;


            default:

                bbDebug("Unknown local message type\n");
            }
        }
    }
}

