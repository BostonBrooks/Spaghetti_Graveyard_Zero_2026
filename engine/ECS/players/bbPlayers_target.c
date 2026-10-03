
#include "engine/ECS/players/bbPlayers_target.h"

#include "print_entity.h"
#include "core/actions.h"
#include "core/action_request.h"
#include "engine/core/bbAction.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction_operations.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/bbECS_instructions.h"
#include "engine/logic/bbTerminal.h"
#include "engine/test_string/bbTestString.h"

bbFlag bbActionRequest_setPlayerTarget(void* Core,
                                       U32 sender,
                                       U32 collision,
                                       bbTime created_tick,
                                       bbTime act_tick,
                                       U32 player,
                                       bbHandle target_server_handle) {

    bbCore* core = (bbCore*)Core;

    bbAction action;
    action.header.type = bbActionType_setPlayerTarget;
    action.header.status = bbAction_Wait;
    action.header.sender = sender;
    action.header.collision = collision;
    action.header.created_tick = created_tick;
    action.header.act_tick = act_tick;
    action.integer = player;
    action.handle = target_server_handle;
    bbAction_request(core,&home.network,&action);

    return bbSuccess;
}

bbFlag bbAction_setPlayerTarget_fn(bbCore* core, bbAction* action) {
    bbNotImplemented()
    bbHandle server_handle = action->handle;
    bbHandle entity_handle;
    bbHandle_mapComponent(core->ECS, bbECS_ServerEntities, server_handle, bbECS_ECS, &entity_handle, NULL);

    bbEntity_print(entity_handle);
    bbHandle action_handle;
    bbVPool_reverseLookup(core->action_pool,action,&action_handle);
    bbCS_setPlayerTarget(core, action->integer, entity_handle, bbInstructionSource_action, action_handle);



    return bbSuccess;
}
bbFlag bbActionRequest_setPlayerAttack(void* Core,
                       U32 sender,
                       U32 collision,
                       bbTime created_tick,
                       bbTime act_tick,
                       U32 player,
                       bbHandle target_server_handle) {


        bbCore* core = (bbCore*)Core;

        bbAction action;
        action.header.type = bbActionType_setPlayerAttack;
        action.header.status = bbAction_Wait;
        action.header.sender = sender;
        action.header.collision = collision;
        action.header.created_tick = created_tick;
        action.header.act_tick = act_tick;
        action.integer = player;
        action.handle = target_server_handle;
        bbAction_request(core,&home.network,&action);

        return bbSuccess;

}

bbFlag bbAction_setPlayerAttack_fn(bbCore* core, bbAction* action) {

    U32 player = action->integer;
    bbHandle target_server_handle = action->handle;
    bbHandle action_handle;
    bbHandle target_handle;
    bbAI_Component* ai_component;
    bbHandle_mapComponent(core->ECS,
                   bbECS_ServerEntities,
                   target_server_handle,
                   bbECS_ECS,
                   &target_handle,NULL);

    bbHandle player_entity_handle = home.ECS.players.players[player].selected_entities[0];

    bbHandle_mapComponent(core->ECS, bbECS_ECS, player_entity_handle, bbECS_AI, NULL, (bbComponent**)&ai_component);

    bbAI_CommandData data;
    data.handle = target_handle;

    bbEntity_print(target_handle)
    bbEntity_print(home.ECS.players.players[player].target_entity)

    bbAI_onCommand(ai_component,
                          (bbAI_System*)home.ECS.ECS->systems[bbECS_AI],
                          bbAI_targetMonster,
                          data,
                          true);

    bbVPool_reverseLookup(core->action_pool,action,&action_handle);

    bbCI_doNothing(core, bbInstructionSource_action, action_handle);

    return bbSuccess;
}




bbFlag bbCI_setPlayerTarget(bbCore* core, U32 player, bbHandle entity_handle, bbInstruction_source source, bbHandle action)
{
    allocActiveInstruction(instruction)

    instruction->type = bbI_setPlayerTarget;
    instruction->data.three_handles.handle1.u64 = player;
    instruction->data.three_handles.handle2 = entity_handle;
    instruction->source = source;
    instruction->redo_instruction = action;

    pushActiveInstruction(instruction)
    return bbSuccess;
}
bbFlag bbCS_setPlayerTarget(bbCore* core, U32 player, bbHandle entity_handle, bbInstruction_source source, bbHandle action)
{

    bbPlayer* player_object = &home.ECS.players.players[player];
    if (source == bbInstructionSource_input)
    {
        //create input instruction
        allocRedoInstruction(instruction)
        instruction->source = source;
        //set input instruction data
        instruction->type = bbI_setPlayerTarget;
        instruction->data.three_handles.handle1.u64 = player;
        instruction->data.three_handles.handle2 = entity_handle;

        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = source;
        undo_instruction->redo_instruction = (bbHandle)instruction_handle;

        //set instruction data
        undo_instruction->type = bbI_unsetPlayerTarget;

        undo_instruction->data.three_handles.handle1.u64 = player;
        undo_instruction->data.three_handles.handle2 = player_object->target_entity;
        pushUndoInstruction(undo_instruction)
        pushRedoInstruction(instruction)
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = source;

        //set instruction data
        undo_instruction->type = bbI_unsetPlayerTarget;
        undo_instruction->data.three_handles.handle1.u64 = player;
        undo_instruction->data.three_handles.handle2 = player_object->target_entity;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        undo_instruction->type = bbI_unsetPlayerTarget;
        undo_instruction->data.three_handles.handle1.u64 = player;
        undo_instruction->data.three_handles.handle2 = player_object->target_entity;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_norewind)
    {

    }

    player_object->target_entity = entity_handle;


    bbEntity_print(entity_handle);

    return bbSuccess;
}

bbFlag bbI_setPlayerTarget_fn(bbCore* core, bbInstruction* instruction)
{
    I32 player = instruction->data.three_handles.handle1.u64;
    bbPlayer* player_object = &home.ECS.players.players[player];

    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unsetPlayerTarget;
        undo_instruction->data.three_handles.handle1.u64 = player;
        undo_instruction->data.three_handles.handle2 = player_object->target_entity;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)


    }
    else if (instruction->source == bbInstructionSource_input)
    {

        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unsetPlayerTarget;
        undo_instruction->data.three_handles.handle1.u64 = player;
        undo_instruction->data.three_handles.handle2 = player_object->target_entity;
        undo_instruction->source = instruction->source;
        allocRedoInstruction(redo_instruction)
        *redo_instruction = *instruction;
        undo_instruction->redo_instruction = (bbHandle)redo_instruction_handle;
        pushRedoInstruction(redo_instruction)
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_action)
    {

        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unsetPlayerTarget;
        undo_instruction->data.three_handles.handle1.u64 = player;
        undo_instruction->data.three_handles.handle2 = player_object->target_entity;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = (bbHandle)instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)

        bbAction* action;
        bbVPool_lookup(core->action_queue.pool, (void**)&action, instruction->redo_instruction);
        printf("collision = %d ", action->header.collision);
    } //else source == no rewind


    player_object->target_entity = instruction->data.three_handles.handle2;
    return bbSuccess;
}
bbFlag bbI_unsetPlayerTarget_fn(bbCore* core, bbInstruction* instruction)
{

    I32 player = instruction->data.three_handles.handle1.u64;
    bbPlayer* player_object = &home.ECS.players.players[player];
    player_object->target_entity = instruction->data.three_handles.handle2;

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
    return bbSuccess;
}