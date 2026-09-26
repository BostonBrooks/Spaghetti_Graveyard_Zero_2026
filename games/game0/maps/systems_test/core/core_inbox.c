#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInbox.h"
#include "core_inbox.h"
#include "games/game0/maps/systems_test/core/send_server_enitity.h"
#include "netpause_button.h"
#include "unfreeze_button.h"
#include "engine/data/bbHome.h"
#include "games/game0/maps/systems_test/core/spawn_entity.h"
#include "engine/ECS/moveables/bbMoveables.h"
#include "engine/logic/bbString.h"
#include "engine/logic/bbSystemPool.h"
#include "engine/ECS/AI_system/bbAI_System.h"

bbFlag bbCoreInbox_clickMonster_fn(bbCore* core, bbCoreInboxMessage* message);
bbFlag bbCoreInbox_receiveMessage_fn(bbCore* core, bbCoreInboxMessage* message);
bbFlag bbCoreInbox_Freeze(bbCore* core)
{
    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_freeze;
    bbThreadedQueue_pushL(&core->local_message_queue, message);

    return bbSuccess;
}


bbFlag bbCoreInbox_ClickUnit(bbCore* core, bbHandle entity_handle, U64 control_keys)
{
    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_clickUnit;
    message->data.three_handles.handle1 = entity_handle;
    message->data.three_handles.handle2.u64 = control_keys;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
}
bbFlag bbCoreInbox_clickUnit_fn(bbCore* core, struct bbCoreInboxMessage* message)
{
    bbNotImplemented()

    bbDebug("clicked entity index %d, control keys:\n %064" PRIb64 "\n",
        message->data.three_handles.handle1.system.index,
        message->data.three_handles.handle2.u64);
}

bbFlag bbCoreInbox_ClicMap(bbCore* core, bbMapCoords coords, U64 control_keys)
{
    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_clickUnit;
    message->data.agent_MC.coords = coords;
    message->data.agent_MC.handle1.u64 = control_keys;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
}
bbFlag bbCoreInbox_clickMap_fn(bbCore* core, struct bbCoreInboxMessage* message)
{
    bbNotImplemented()

    bbDebug("clicked map coords (%d, %d, %d)), control keys:\n %064" PRIb64 "\n",
    message->data.agent_MC.coords.i,
    message->data.agent_MC.coords.j,
    message->data.agent_MC.coords.k,
    message->data.agent_MC.handle1.u64);
}

bbFlag bbCoreInbox_KeyPress(bbCore* core, U64 key, U64 control_keys)
{
    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_keyPress;
    message->data.three_handles.handle1.u64 = key;
    message->data.three_handles.handle2.u64 = control_keys;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
}
bbFlag bbCoreInbox_keyPress_fn(bbCore* core, struct bbCoreInboxMessage* message)
{
    bbNotImplemented()

bbDebug("clicked key %llu, control keys:\n %064" PRIb64 "\n",
    message->data.three_handles.handle1.u64,
    message->data.three_handles.handle2.u64);
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


    bbPlayers* players = &home.ECS.players;
    bbHandle player_character = players->players[players->this_player].selected_entities[0];
    //
    if (player_character.u64 == 0) return bbNone;

    bbFlag flag = bbHandle_mapComponent(home.ECS.ECS, bbECS_ECS,player_character,
        bbECS_AI,&ai_handle,(bbComponent**)&component);
    //
    if (flag!= bbSuccess) return bbNone;
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
bbFlag bbCoreInbox_clickPlayer_fn(bbCore* core, bbCoreInboxMessage* message);
bbFlag bbCore_initInboxMessages(bbCore* core)
{
    core->inbox_functions = calloc(bbCoreInbox_numVTypes-bbCoreInbox_numTypes,sizeof(bbCoreInbox_fn*));
    core->inbox_functions[bbCoreInbox_testMessage-bbCoreInbox_numTypes] = bbCoreInbox_Test_fn;
    core->inbox_functions[bbCoreInbox_netpauseButton-bbCoreInbox_numTypes] = bbCoreInbox_netpauseButton_fn;
    core->inbox_functions[bbCoreInbox_unfreezeButton-bbCoreInbox_numTypes] = bbCoreInbox_unfreezeButton_fn;
    core->inbox_functions[bbCoreInbox_testClick-bbCoreInbox_numTypes] = bbCoreInbox_testClick_fn;
    core->inbox_functions[bbCoreInbox_testClick2-bbCoreInbox_numTypes] = bbCoreInbox_testClick2_fn;
    core->inbox_functions[bbCoreInbox_testClick3-bbCoreInbox_numTypes] = bbCoreInbox_testClick3_fn;
    core->inbox_functions[bbCoreInbox_setGoalpoint-bbCoreInbox_numTypes] = bbCoreInbox_setGoalpoint_fn;
    core->inbox_functions[bbCoreInbox_freeze-bbCoreInbox_numTypes] = bbCoreInbox_Freese_fn;
    core->inbox_functions[bbCoreInbox_receiveMessage-bbCoreInbox_numTypes] = bbCoreInbox_receiveMessage_fn;
    core->inbox_functions[bbCoreInbox_clickPlayer-bbCoreInbox_numTypes] = bbCoreInbox_clickPlayer_fn;
    core->inbox_functions[bbCoreInbox_clickMonster-bbCoreInbox_numTypes] = bbCoreInbox_clickMonster_fn;
   core->inbox_functions[bbCoreInbox_setPlayerEntity-bbCoreInbox_numTypes] = bbCoreInbox_setPlayerEntity_fn;
   core->inbox_functions[bbCoreInbox_clickMap-bbCoreInbox_numTypes] =  bbCoreInbox_clickMap_fn;
   core->inbox_functions[bbCoreInbox_clickUnit-bbCoreInbox_numTypes] = bbCoreInbox_clickUnit_fn;
   core->inbox_functions[bbCoreInbox_keyPress-bbCoreInbox_numTypes] = bbCoreInbox_keyPress_fn;
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

bbFlag bbCoreInbox_ReceiveMessage(bbCore* core,bbNetwork* Network, bbHandle threaded_pool_handle) {

    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_receiveMessage;
    message->data.three_handles.handle1 = threaded_pool_handle;
    bbThreadedQueue_pushL(&core->local_message_queue, message);

    return bbSuccess;
}

bbFlag bbCoreInbox_receiveMessage_fn(bbCore* core, bbCoreInboxMessage* message)
{
    bbHandle threaded_pool_handle = message->data.three_handles.handle1;
    //bbDebug("threaded pool handle = %llu\n", threaded_pool_handle.u64);
    bbTextbox_message* message_in;
    bbFlag flag = bbVPool_lookup(home.textbox_app.textbox_system.threaded_pool,(void**)&message_in, threaded_pool_handle);
    //bbFlag_print(flag)

    bbHandle handle_out;
    bbTextbox_message* message_out;
    bbVPool_alloc2(home.textbox_app.textbox_system.pool,(void**)&message_out, &handle_out);
    //bbDebug("system handle = %llu\n", handle_out.u64);
    message_out->timestamp = message_in->timestamp;
    message_out->length = message_in->length;
    message_out->type = message_in->type;
    bbStr_setStr(message_out->text,message_in->text, MESSAGE_LENGTH);

    //bbDebug("message_out->text = %s\n", message_out->text);
    bbTextbox_putMessage(home.textbox_app.textboxes[bbTextbox_Dialogue],handle_out,100000);
    bbTextbox_updateBuffer(home.textbox_app.textboxes[bbTextbox_Dialogue]);

    bbThreadedPool* pool = home.textbox_app.textbox_system.threaded_pool->pool;
    //bbDebug("available = %d\n", pool->num - pool->in_use);
    bbVPool_free(home.textbox_app.textbox_system.threaded_pool,message_in);
    //bbDebug("available = %d\n", pool->num - pool->in_use);
    return bbSuccess;
}


bbFlag bbCoreInbox_ClickMonster(bbCore* core, bbHandle entity_handle)
{
    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_clickMonster;
    message->data.three_handles.handle1 = entity_handle;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
}

bbFlag bbCoreInbox_clickMonster_fn(bbCore* core, bbCoreInboxMessage* message)
{
    bbHandle ai_handle;
    bbAI_Component* component;
    //
    bbPlayers* players = &home.ECS.players;
    bbHandle player_character = players->players[players->this_player].selected_entities[0];

    bbFlag flag = bbHandle_mapComponent(home.ECS.ECS, bbECS_ECS,player_character,
        bbECS_AI,&ai_handle,(bbComponent**)&component);

    if (flag != bbSuccess) return flag;
    bbAI_CommandData data;
    data.handle = message->data.three_handles.handle1;


    bbAI_onCommand(component,
                  (bbAI_System*)home.ECS.ECS->systems[bbECS_AI],
                  bbAI_clickMonster,
                  data,
                  false);
}


bbFlag bbCoreInbox_ClickPlayer(bbCore* core, bbHandle entity_handle)
{
    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_clickPlayer;
    message->data.three_handles.handle1 = entity_handle;
    bbThreadedQueue_pushL(&core->local_message_queue, message);
}

bbFlag bbCoreInbox_clickPlayer_fn(bbCore* core, bbCoreInboxMessage* message)
{
    bbHandle server_handle;
    bbAI_Component* component;

    bbHandle_mapComponent(home.ECS.ECS, bbECS_ECS,message->data.three_handles.handle1,
        bbECS_ServerEntities,&server_handle,(bbComponent**)&component);

    bbActionRequest_setPlayerEntity(&home.core.core,
                       home.ECS.players.this_player,
                       7,
                       home.core.core.actual_time,
                       home.core.core.actual_time,
                       home.ECS.players.this_player,
                       server_handle);

}