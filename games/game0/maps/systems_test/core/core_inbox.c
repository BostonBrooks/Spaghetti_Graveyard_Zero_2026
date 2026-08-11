#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInbox.h"
#include "core_inbox.h"

#include "netpause_button.h"
#include "unfreeze_button.h"
#include "engine/ECS/spawn_entity.h"

bbFlag bbCoreInboxTest_fn(bbCore* core, struct bbCoreInboxMessage* message)
{
    bbHere();
    return bbSuccess;
}

bbFlag bbCore_initInboxMessages(bbCore* core)
{
    core->inbox_functions = calloc(16,sizeof(bbCoreInbox_fn*));
    core->inbox_functions[bbCoreInbox_testMessage-bbCoreInbox_numTypes] = bbCoreInboxTest_fn;
    core->inbox_functions[bbCoreInbox_netpauseButton-bbCoreInbox_numTypes] = bbCoreInbox_netpauseButton_fn;
    core->inbox_functions[bbCoreInbox_unfreezeButton-bbCoreInbox_numTypes] = bbCoreInbox_unfreezeButton_fn;
    core->inbox_functions[bbCoreInbox_testClick-bbCoreInbox_numTypes] = bbCoreInbox_testClick_fn;
    core->inbox_functions[bbCoreInbox_testClick2-bbCoreInbox_numTypes] = bbCoreInbox_testClick2_fn;

    return bbSuccess;
}

bbFlag bbCoreInbox_TestMessage(bbCore* core)
{
    bbHere();
    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_testMessage;
    bbThreadedQueue_pushL(&core->local_message_queue, message);

    return bbSuccess;
}

bbFlag bbCoreInbox_TestClick(bbCore* core, bbMapCoords MC)
{
    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_testClick;
    message->data.map_coords = MC;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}