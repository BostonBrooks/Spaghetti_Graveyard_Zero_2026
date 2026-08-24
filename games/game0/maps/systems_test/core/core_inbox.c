#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInbox.h"
#include "core_inbox.h"

#include "netpause_button.h"
#include "unfreeze_button.h"
#include "engine/data/bbHome.h"
#include "games/game0/maps/systems_test/core/spawn_entity.h"
#include "engine/ECS/moveables/bbMoveables.h"


bbFlag bbCoreInbox_Freeze(bbCore* core)
{
    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_freeze;
    bbThreadedQueue_pushL(&core->local_message_queue, message);

    return bbSuccess;
}


bbFlag bbCoreInbox_Freese_fn(bbCore* core, struct bbCoreInboxMessage* message)
{
    bbHere();
    sfSleep(sfSeconds(1.f));
    return bbSuccess;
}

bbFlag bbCoreInbox_Test_fn(bbCore* core, struct bbCoreInboxMessage* message)
{
    bbHere();
    return bbSuccess;
}
bbFlag bbCoreInbox_testClick_fn(bbCore* core, bbCoreInboxMessage* message)
{

    bbHandle ai_handle;
    bbAI_Component* component;
    //
    bbHandle_mapComponent(home.ECS.ECS, bbECS_ECS,home.ECS.ECS->player_character,
        bbECS_AI,&ai_handle,(bbComponent**)&component);
    //
    bbAI_CommandData data;
    data.goal_point = message->data.map_click.coords;
    data.integer = message->data.map_click.button;

    bbAI_onCommand(component,
                  (bbAI_System*)home.ECS.ECS->systems[bbECS_AI],
                  bbAI_mapClick,
                  data,
                  false);

    return bbSuccess;
}
bbFlag bbCoreInbox_setGoalpoint_fn(bbCore* core, bbCoreInboxMessage* message);

bbFlag bbCore_initInboxMessages(bbCore* core)
{
    core->inbox_functions = calloc(16,sizeof(bbCoreInbox_fn*));
    core->inbox_functions[bbCoreInbox_testMessage-bbCoreInbox_numTypes] = bbCoreInbox_Test_fn;
    core->inbox_functions[bbCoreInbox_netpauseButton-bbCoreInbox_numTypes] = bbCoreInbox_netpauseButton_fn;
    core->inbox_functions[bbCoreInbox_unfreezeButton-bbCoreInbox_numTypes] = bbCoreInbox_unfreezeButton_fn;
    core->inbox_functions[bbCoreInbox_testClick-bbCoreInbox_numTypes] = bbCoreInbox_testClick_fn;
    core->inbox_functions[bbCoreInbox_testClick2-bbCoreInbox_numTypes] = bbCoreInbox_testClick2_fn;
    core->inbox_functions[bbCoreInbox_setGoalpoint-bbCoreInbox_numTypes] = bbCoreInbox_setGoalpoint_fn;
    core->inbox_functions[bbCoreInbox_freeze-bbCoreInbox_numTypes] = bbCoreInbox_Freese_fn;
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