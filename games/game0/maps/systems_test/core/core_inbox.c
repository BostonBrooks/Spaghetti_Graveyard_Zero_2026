#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInbox.h"
#include "core_inbox.h"

#include "netpause_button.h"
#include "unfreeze_button.h"
#include "engine/data/bbHome.h"
#include "games/game0/maps/systems_test/core/spawn_entity.h"
#include "engine/ECS/moveables/bbMoveables.h"

bbFlag bbCoreInboxTest_fn(bbCore* core, struct bbCoreInboxMessage* message)
{
    bbHere();
    return bbSuccess;
}

bbFlag bbCoreInbox_setGoalpoint_fn(bbCore* core, bbCoreInboxMessage* message);

bbFlag bbCore_initInboxMessages(bbCore* core)
{
    core->inbox_functions = calloc(16,sizeof(bbCoreInbox_fn*));
    core->inbox_functions[bbCoreInbox_testMessage-bbCoreInbox_numTypes] = bbCoreInboxTest_fn;
    core->inbox_functions[bbCoreInbox_netpauseButton-bbCoreInbox_numTypes] = bbCoreInbox_netpauseButton_fn;
    core->inbox_functions[bbCoreInbox_unfreezeButton-bbCoreInbox_numTypes] = bbCoreInbox_unfreezeButton_fn;
    core->inbox_functions[bbCoreInbox_testClick-bbCoreInbox_numTypes] = bbCoreInbox_testClick_fn;
    core->inbox_functions[bbCoreInbox_testClick2-bbCoreInbox_numTypes] = bbCoreInbox_testClick2_fn;
    core->inbox_functions[bbCoreInbox_setGoalpoint-bbCoreInbox_numTypes] = bbCoreInbox_setGoalpoint_fn;

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

bbFlag bbCoreInbox_TestClick(bbCore* core, bbMapCoords MC, I32 button)
{
    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_testClick;
    message->data.map_click.coords = MC;
    message->data.map_click.button = button;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}



bbFlag bbCoreInbox_SetGoalpoint(bbCore* core, bbHandle entity, bbMapCoords goalpoint)
{
    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_setGoalpoint;
    message->data.agent_MC.coords = goalpoint;
    message->data.agent_MC.handle1 = entity;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
}


bbFlag bbCoreInbox_setGoalpoint_fn(bbCore* core, bbCoreInboxMessage* message)
{

    bbMoveable_setGoalPoint(&home.ECS.moveables,message->data.agent_MC.handle1, message->data.agent_MC.coords);
}