#include "core_inbox.h"
#include "action_request.h"
#include "engine/core/bbCoreInbox.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbString.h"
#include "engine/threadsafe/bbThreadedQueue.h"
#include "games/game0/maps/systems_test/core/actions.h"
extern U32 collision;


bbFlag bbCoreInbox_TestClick3(bbCore* core, bbHandle entity_handle)
{

    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_testClick3;
    message->act_time = home.core.core.actual_time;
    message->data.three_handles.handle1 = entity_handle;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}



bbFlag bbCoreInbox_testClick3_fn(bbCore* core, bbCoreInboxMessage* message)
{
    bbAction action;

    bbHandle server_handle;
    bbHandle entity_handle = message->data.three_handles.handle1;

    bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS,entity_handle,bbECS_ServerEntities,&server_handle,NULL);


    action.header.type = bbActionType_setTarget;
    action.header.status = bbAction_Wait;
    action.header.sender = 0;
    action.header.collision = collision++;
    action.header.created_tick = home.core.core.actual_time;
    action.header.act_tick = home.core.core.actual_time;
    bbStr_setStr(action.header.key,"ACHILLES", KEY_LENGTH);
    action.handle = server_handle;

    bbCoreInput_requestAction(&home.core.core,&home.network,&action,home.core.core.actual_time,
                                bbInstructionSource_internal, no_handle);
    return bbSuccess;
}
