#include "engine/ECS/players/bbPlayers.h"

#include "core/core_inbox.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInbox.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/bbECS.h"
#include "engine/logic/bbSystemPool.h"
#include "engine/logic/bbTerminal.h"

bbFlag bbPlayers_getComponent_fn(struct bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    return bbVPool_lookup(system->pool, (void**)component, component_handle);
}
bbFlag bbPlayers_getHandle_fn(struct bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    return bbVPool_reverseLookup(system->pool, (void*)component, component_handle);
}

bbFlag bbPlayers_init(bbPlayers* system, bbECS* ECS){

    bbVPool_newSystem(&system->system.pool,bbECS_Players,sizeof(bbPlayer),10,100,"PLAYER COMPONENTS");
    system->this_player = -1;

    for (I32 i = 0; i < num_players; i++) {
        for (I32 j = 0; j < num_selected_entities; j++) {
            system->players[i].selected_entities[j] = system->system.pool->null;
        }
    }

    system->system.getComponent = bbPlayers_getComponent_fn;
    system->system.getHandle = bbPlayers_getHandle_fn;
    system->system.delete = NULL;
    system->system.ECS = ECS;

    ECS->systems[bbECS_Players] = (bbSystem* )system;

    return bbSuccess;
}

bbFlag bbAction_setPlayerEntity(void* Core,
                       U32 sender,
                       U32 collision,
                       U64 created_tick,
                       U64 act_tick,
                       U32 player,
                       bbHandle server_handle) {
    bbNotImplemented() //request action from server


    bbDebug("You clicked server handle %d", server_handle.system.index);
    bbCore* core = (bbCore*)Core;
    bbECS* ECS = core->ECS;
    bbPlayers* players = (bbPlayers*)ECS->systems[bbECS_Players];

    bbHandle entity_handle;

    bbHandle_mapComponent(ECS, bbECS_ServerEntities, server_handle,bbECS_ECS,&entity_handle,NULL);

    bbUI_Inbox_SetViewpoint(&home.UI.inbox, entity_handle);

    players->players[player].selected_entities[0] = entity_handle;



    for (I32 i = 1; i < num_selected_entities; i++) {
        players->players[player].selected_entities[0] = ECS->system.pool->null;
    }

    return bbSuccess;

}


bbFlag bbCoreInbox_SetPlayerEntity(bbCore* core, U32 player, bbHandle server_handle) {

    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_setPlayerEntity; //TODO I dont want to  #include "core/core_inbox.h"
    message->data.three_handles.handle1.u64 = player;
    message->data.three_handles.handle2 = server_handle;
    bbThreadedQueue_pushL(&core->local_message_queue, message);

    return bbSuccess;
}
bbFlag bbCoreInbox_setPlayerEntity_fn(bbCore* core, bbCoreInboxMessage* message) {
    bbAction_setPlayerEntity(core,
                       0,
                       0,
                       0,
                       0,
                       message->data.three_handles.handle1.u64, //player index
                       message->data.three_handles.handle2); //server handle
    return bbSuccess;
}