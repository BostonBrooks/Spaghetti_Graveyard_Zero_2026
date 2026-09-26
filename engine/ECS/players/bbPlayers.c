#include "engine/ECS/players/bbPlayers.h"

#include "core/actions.h"
#include "core/action_request.h"
#include "core/core_inbox.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInbox.h"
#include "engine/core/bbInstruction_operations.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/bbECS.h"
#include "engine/ECS/bbECS_instructions.h"
#include "engine/logic/bbSystemPool.h"
#include "engine/logic/bbTerminal.h"

extern U32 collision;

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
    system->this_player = 0;

    for (I32 i = 0; i < NUM_PLAYERS; i++) {
        for (I32 j = 0; j < MAX_SELECTED_ENTITIES; j++) {
            system->players[i].selected_entities[j] = system->system.pool->null;
            system->players[i].state = bbPlayer_stateNULL;
        }
    }
    system->system.getComponent = bbPlayers_getComponent_fn;
    system->system.getHandle = bbPlayers_getHandle_fn;
    system->system.delete = NULL;
    system->system.ECS = ECS;

    ECS->systems[bbECS_Players] = (bbSystem* )system;

    bbPlayers_populate(system);

    return bbSuccess;
}


bbFlag bbPlayer_ClickMap(bbPlayers* players, bbMapCoords coords, U64 control_keys) {
    I32 this_player_int = players->this_player;
    bbPlayer* this_player = &players->players[this_player_int];
    I32 state = this_player->state;
    bbPlayerState player_state = players->states[state];
    player_state.click_map(players,coords,control_keys);
}
bbFlag bbPlayer_ClickUnit(bbPlayers* players, bbHandle entity_handle, U64 control_keys)
{
    I32 this_player_int = players->this_player;
    bbPlayer* this_player = &players->players[this_player_int];
    I32 state = this_player->state;
    bbPlayerState player_state = players->states[state];
    player_state.click_unit(players,entity_handle,control_keys);
}
bbFlag bbPlayer_KeyPress(bbPlayers* players, U64 key, U64 control_keys)
{
    I32 this_player_int = players->this_player;
    bbPlayer* this_player = &players->players[this_player_int];
    I32 state = this_player->state;
    bbPlayerState player_state = players->states[state];
    player_state.key_press(players,key,control_keys);
}


bbFlag bbCoreInput_setPlayerState(bbCore* core, U32 player_int, U32 state) {
    bbNotImplemented()

    bbPlayers* players = (bbPlayers*)core->ECS->systems[bbECS_Players];
    bbPlayer* player = &players->players[player_int];
    player->state = state;
    if (player_int == players->this_player) {
        bbInputModes_set(&home.UI.input_modes,
            players->states[state].bbInputMode_key);
    }
    return bbSuccess;
}

bbFlag bbCoreInbox_SetPlayerEntity(bbCore* core, U32 player, bbHandle entity_handle) {

    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_setPlayerEntity;
    message->data.three_handles.handle1.u64 = player;
    message->data.three_handles.handle2 = entity_handle;
    bbThreadedQueue_pushL(&core->local_message_queue, message);

    return bbSuccess;
}

bbFlag bbCoreInbox_setPlayerEntity_fn(bbCore* core, bbCoreInboxMessage* message) {

    bbHandle entity_handle = message->data.three_handles.handle2;
    bbHandle server_handle;
    bbFlag flag = bbHandle_mapComponent(core->ECS,bbECS_ECS,entity_handle,bbECS_ServerEntities,&entity_handle,NULL);
    bbFlag_print(flag)



    bbActionRequest_setPlayerEntity(core,
                       home.network.server_socket_number,
                       collision++,
                       core->actual_time,
                       core->actual_time,
                       message->data.three_handles.handle1.u64, //player index
                       server_handle); //server handle
    return bbSuccess;
}

bbFlag bbActionRequest_setPlayerEntity(void* Core,
                       U32 sender,
                       U32 collision,
                       U64 created_tick,
                       U64 act_tick,
                       U32 player,
                       bbHandle server_handle) {


    bbCore* core = (bbCore*)Core;

    bbAction action;
    action.header.type = bbActionType_setPlayerEntity;
    action.header.status = bbAction_Wait;
    action.header.sender = sender;
    action.header.collision = collision;
    action.header.created_tick = created_tick;
    action.header.act_tick = act_tick;
    action.integer = player;
    action.handle = server_handle;

    bbECS_entity* entity;
    bbHandle_mapComponent(home.ECS.ECS, bbECS_ServerEntities,server_handle,bbECS_ECS,NULL,(bbComponent**)&entity);
    bbDebug("player = %u, set entity %s\n", action.integer,entity->key);

    bbAction_request(core,&home.network,&action);

    return bbSuccess;

}

bbFlag bbAction_setPlayerEntity_fn(bbCore* core, bbAction* action)
{//bbHere()

    I32 player_index = action->integer;
    bbHandle server_handle = action->handle;
    bbHandle entity_handle;
    bbECS_entity* entity;

    bbFlag flag = bbHandle_mapComponent(core->ECS,
        bbECS_ServerEntities,
        server_handle,
        bbECS_ECS,
        &entity_handle,
        (bbComponent**)&entity);

    //bbFlag_print(flag)
    //bbDebug("player %d clicked %s\n",player_index,entity->key);


    bbHandle action_handle;
    bbVPool_reverseLookup(core->action_pool,action,&action_handle);

    bbCI_setPlayerEntity(core,
                         player_index,
                         entity_handle,
                         bbInstructionSource_action,
                         action_handle);
    return bbSuccess;
}

bbFlag bbCI_setPlayerEntity(bbCore* core, I32 player_index, bbHandle entity_handle,  bbInstruction_source source, bbHandle action)
{
    allocActiveInstruction(instruction)
    instruction->type = bbI_setPlayerEntity;
    instruction->source = source;
    instruction->redo_instruction = action;
    instruction->data.three_handles.handle1.u64 = player_index;
    instruction->data.three_handles.handle2 = entity_handle;
    pushActiveInstruction(instruction)

    // bbECS_entity* entity;
    // bbHandle_getComponent(&home.ECS.ECS->system,(bbComponent**)&entity,entity_handle);
    //
    // bbDebug("player index = %d, entity %s\n", player_index, entity->key);
    return bbSuccess;
}

///Modify player entity
bbFlag bbI_setPlayerEntity_fn(bbCore* core, bbInstruction* instruction) {

    I32  player_index = instruction->data.three_handles.handle1.u64;
    bbHandle  entity_handle = instruction->data.three_handles.handle2;

    // bbECS_entity*  entity;
    // bbHandle_getComponent(&home.ECS.ECS->system, (bbComponent**)&entity, entity_handle);
    //
    // bbDebug("player index = %d, selected entity = %s\n", player_index, entity->key);


    bbPlayers* players = (bbPlayers*) core->ECS->systems[bbECS_Players];
    bbPlayer player = players->players[player_index];

    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_unsetPlayerEntity;
        undo_instruction->data.three_handles.handle1.u64 = player_index;
        undo_instruction->data.three_handles.handle2 = player.selected_entities[0];

        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_unsetPlayerEntity;
        undo_instruction->data.three_handles.handle1.u64 = player_index;
        undo_instruction->data.three_handles.handle2 = player.selected_entities[0];
        undo_instruction->source = instruction->source;
        allocRedoInstruction(redo_instruction)
         *redo_instruction = *instruction;
        undo_instruction->redo_instruction = (bbHandle)redo_instruction_handle;
        pushRedoInstruction(redo_instruction)
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_action)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_unsetPlayerEntity;
        undo_instruction->data.three_handles.handle1.u64 = player_index;
        undo_instruction->data.three_handles.handle2 = player.selected_entities[0];
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)

    }


    player.selected_entities[0] = entity_handle;

    for (I32 i = 1; i < MAX_SELECTED_ENTITIES; i++) {
        player.selected_entities[i] = core->ECS->system.pool->null;
    }
    if (home.ECS.players.this_player == player_index)
    {
        bbUI_Inbox_SetViewpoint(&home.UI.inbox, entity_handle);
    }
    players->players[player_index].selected_entities[0] = entity_handle;

    // bbHandle entity_handle2 = players->players[player_index].selected_entities[0];
    // bbHandle_getComponent(&home.ECS.ECS->system, (bbComponent**)&entity, entity_handle2);
    //
    // bbDebug("player index = %d, selected entity = %s\n", player_index, entity->key);
    return bbSuccess;
}

///Rollback modification
bbFlag bbI_unsetPlayerEntity_fn(bbCore* core, bbInstruction* instruction) {
    I32  player_index = instruction->data.three_handles.handle1.u64;
    bbHandle  entity_handle = instruction->data.three_handles.handle2;

    bbPlayers* players = (bbPlayers*)core->ECS->systems[bbECS_Players];
    bbPlayer player = players->players[player_index];
    player.selected_entities[0] = entity_handle;
    for (I32 i = 1; i < MAX_SELECTED_ENTITIES; i++) {
        player.selected_entities[i] = core->ECS->system.pool->null;
    }
    if (home.ECS.players.this_player == player_index)
    {
        bbUI_Inbox_SetViewpoint(&home.UI.inbox, entity_handle);
    }

    if (instruction->source == bbInstructionSource_internal)
    {
        //bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {

        popRedoInstruction(redo_instruction,instruction)
        allocActiveInstruction(new_instruction)
        *new_instruction = redo_instruction;
        bbCore_checkMap(&core->map, &redo_instruction, instruction);

        pushActiveInstruction(new_instruction)
        //bbInstruction* redo_instruction;
        //bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        //bbList_pushL(&core->active_stack, redo_instruction);
        //bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        //bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    bbAssert(0==1, "We should not get here\n");
}