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
    system->this_player =0;

    for (I32 i = 0; i < num_players; i++) {
        for (I32 j = 0; j < MAX_SELECTED_ENTITIES; j++) {
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

    bbDebug("player = %d\n", player)
    bbAction_request(core,&home.network,&action);

    bbDebug("You clicked server handle %d\n", server_handle.system.index);

    bbECS* ECS = core->ECS;
    bbPlayers* players = (bbPlayers*)ECS->systems[bbECS_Players];

    bbHandle entity_handle;

    bbHandle_mapComponent(ECS, bbECS_ServerEntities, server_handle,bbECS_ECS,&entity_handle,NULL);

    bbUI_Inbox_SetViewpoint(&home.UI.inbox, entity_handle);

    players->players[player].selected_entities[0] = entity_handle;

    bbDebug("entity_handle = %d\n", entity_handle.system.index);


    for (I32 i = 1; i < MAX_SELECTED_ENTITIES; i++) {
        players->players[player].selected_entities[i] = ECS->system.pool->null;
    }

    return bbSuccess;

}
bbFlag bbAction_setPlayerEntity_fn(bbCore* core, bbAction* action)
{bbHere()

    I32 player_index = action->integer;
    bbHandle server_handle = action->handle;

    bbDebug("player = %d\n", player_index)
    bbHandle entity_handle;


    bbHandle_mapComponent(core->ECS, bbECS_ServerEntities, server_handle,bbECS_ECS,&entity_handle,NULL);bbHandle action_handle;
    bbVPool_reverseLookup(core->action_pool,action,&action_handle);

    bbCI_setPlayerEntity(core, player_index,entity_handle,bbInstructionSource_action,action_handle);
    return bbSuccess;
}

bbFlag bbCoreInbox_SetPlayerEntity(bbCore* core, U32 player, bbHandle server_handle) {

    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_setPlayerEntity; //TODO I dont want to  #include "core/core_inbox.h"
    message->data.three_handles.handle1.u64 = player;


    bbDebug("player = %d\n", player)
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

bbFlag bbCI_setPlayerEntity(bbCore* core, I32 player_index, bbHandle entity_handle,  bbInstruction_source source, bbHandle action)
{
    allocActiveInstruction(instruction)
    instruction->type = bbI_setPlayerEntity;
    instruction->source = source;
    instruction->redo_instruction = action;
    instruction->data.three_handles.handle1.u64 = player_index;
    instruction->data.three_handles.handle2 = entity_handle;

    bbDebug("player = %d\n", player_index)

    pushActiveInstruction(instruction)
    return bbSuccess;
}

bbFlag bbI_setPlayerEntity_fn(bbCore* core, bbInstruction* instruction)
{
bbHere()
    I32  player_index = instruction->data.three_handles.handle1.u64;
    bbHandle  entity_handle = instruction->data.three_handles.handle2;

    bbDebug("player = %d\n", player_index)
    bbPlayers* players = (bbPlayers*)core->ECS->systems[bbECS_Players];
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

        // bbHandle handle;
        // bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        // undo_instruction->redo_instruction = handle;
        // bbList_pushL(&core->undo_stack, (void*)undo_instruction);
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

    } //else source == no rewind

    bbDebug("player index = %d, entity_handle = %d, system = %d\n",
        player_index,entity_handle.system.index,entity_handle.system.system);

    player.selected_entities[0] = entity_handle;

    bbUI_Inbox_SetViewpoint(&home.UI.inbox, entity_handle);
    for (I32 i = 1; i < MAX_SELECTED_ENTITIES; i++) {
        player.selected_entities[i] = core->ECS->system.pool->null;
    }

    return bbSuccess;
}
bbFlag bbI_unsetPlayerEntity_fn(bbCore* core, bbInstruction* instruction)
{
    I32  player_index = instruction->data.three_handles.handle1.u64;
    bbHandle  entity_handle = instruction->data.three_handles.handle2;

    bbPlayers* players = (bbPlayers*)core->ECS->systems[bbECS_Players];
    bbPlayer player = players->players[player_index];
    player.selected_entities[0] = entity_handle;


    bbUI_Inbox_SetViewpoint(&home.UI.inbox, entity_handle);
    bbDebug("player index = %d, entity_handle = %d\n", player_index,entity_handle.system.index);

    for (I32 i = 1; i < MAX_SELECTED_ENTITIES; i++) {
        player.selected_entities[i] = core->ECS->system.pool->null;
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
