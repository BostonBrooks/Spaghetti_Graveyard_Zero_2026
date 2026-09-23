
#include "core/instructions.h"
#include "engine/core/bbAction.h"
#include "engine/core/bbInstruction_operations.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/AI_system/bbAI_System.h"
#include "games/game0/maps/ECS_test/virtual_instructions/instructions.h"

bbFlag bbCS_updateAI(bbCore* core, bbInstruction_source source, bbHandle action)
{

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        allocRedoInstruction(instruction)

        //set input instruction data
        instruction->type = bbInstruction_updateAI;
        instruction->source = bbInstructionSource_input;

        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction_handle;

        //set instruction data
        undo_instruction->type = bbInstruction_unupdateAI;
        pushRedoInstruction(instruction)
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = source;

        //set instruction data
        undo_instruction->type = bbInstruction_unupdateAI;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        undo_instruction->type = bbInstruction_unupdateAI;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_norewind)
    {

    }

    bbAI_System_update((bbAI_System*) core->ECS->systems[bbECS_AI]);
    return bbSuccess;
}


bbFlag bbCI_updateAI(bbCore* core, bbInstruction_source source, bbHandle action)
{
    allocActiveInstruction(instruction)

    instruction->type = bbInstruction_updateAI;
    instruction->source = source;
    instruction->redo_instruction = action;

    pushActiveInstruction(instruction)
    return bbSuccess;
}



bbFlag bbI_updateAI_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbInstruction_unupdateAI;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbInstruction_unupdateAI;
        undo_instruction->source = instruction->source;
        allocRedoInstruction(redo_instruction)
         *redo_instruction = *instruction;
        undo_instruction->redo_instruction = redo_instruction_handle;
        pushRedoInstruction(redo_instruction)
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_action)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbInstruction_unupdateAI;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)

    } //else source == no rewind


    bbAI_System_update((bbAI_System*) core->ECS->systems[bbECS_AI]);

    return bbSuccess;
}
bbFlag bbI_unupdateAI_fn(bbCore* core, bbInstruction* instruction)
{


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


bbFlag bbCI_spawnAIComponent2(bbCore* core,
                             bbHandle entity,
                             I32 update,
                             I32 command,
                             bbInstruction_source source,
                             bbHandle action)
{

    allocActiveInstruction(instruction)
    instruction->type = bbI_spawnAIComponent2;
    instruction->data.three_handles.handle1 = entity;
    instruction->data.three_handles.handle2.bloated.index = update;
    instruction->data.three_handles.handle2.bloated.collision = command;
    instruction->source = source;
    instruction->redo_instruction = action;
    pushActiveInstruction(instruction)
    return bbSuccess;
}


bbFlag bbI_spawnAIComponent2_fn(bbCore* core, bbInstruction* instruction)
{
    bbAI_Component* component;
    bbHandle component_handle;

    bbList_alloc2(&home.ECS.AI_system.list,(void**)&component, &component_handle);
    component->ftable.update = instruction->data.three_handles.handle2.bloated.index;
    component->ftable.command = instruction->data.three_handles.handle2.bloated.collision;
    component->state = bbAIState_Idle;

    component->last_attack = 0;
    component->component.entity_handle = instruction->data.three_handles.handle1;

    bbList_pushL(&home.ECS.AI_system.list,component);

    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_unspawnAIComponent2;
        undo_instruction->data.three_handles.handle1 = component_handle;
        undo_instruction->source = instruction->source;
        //bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_unspawnAIComponent2;
        undo_instruction->data.three_handles.handle1 = component_handle;
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
        undo_instruction->type = bbI_unspawnAIComponent2;
        undo_instruction->data.three_handles.handle1 = component_handle;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)
    } //else source == no rewind





    bbCS_entity_setComponent(core,
                             home.ECS.ECS,
                             component->component.entity_handle,
                             component_handle,
                             bbECS_AI,
                             bbInstructionSource_internal,
                             no_handle);

    return bbSuccess;
}

bbFlag bbI_unspawnAIComponent2_fn(bbCore* core, bbInstruction* instruction)
{
    bbHandle AI_handle = instruction->data.three_handles.handle1;

    bbComponent* component;
    bbVPool_lookup(home.ECS.AI_system.system.pool, (void**)&component, AI_handle);

    bbList_remove(&home.ECS.AI_system.list,component);
    bbVPool_free(home.ECS.AI_system.system.pool,component);

    return bbSuccess;

}


bbFlag bbCS_spawnAIComponent2(bbCore* core,
                             bbECS* ECS,
                             bbHandle entity,
                             I32 update,
                             I32 command,
                             bbAI_Component** this,
                             bbInstruction_source source,
                             bbHandle action)
{
    bbAI_Component* component;
    bbHandle component_handle;

    bbAI_System* AI_System = (bbAI_System*)ECS->systems[bbECS_AI];
    bbList_alloc2(&AI_System->list,(void**)&component, &component_handle);
    component->ftable.command = command;
    component->ftable.update = update;
    component->state = bbAIState_Idle;
    component->component.entity_handle = entity;
    component->last_attack = 0;

    bbList_pushL(&AI_System->list,component);

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        allocRedoInstruction(instruction)

        //set input instruction data
        instruction->type = bbI_spawnAIComponent2;
        instruction->data.three_handles.handle1 = entity;
        instruction->data.three_handles.handle2.bloated.index = update;
        instruction->data.three_handles.handle2.bloated.collision = command;

        instruction->source = source;
        instruction->redo_instruction = action;
        //bbStr_setStr(instruction->data.key, string, KEY_LENGTH);

        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction_handle;

        //set instruction data
        undo_instruction->type = bbI_unspawnAIComponent2;
        undo_instruction->data.three_handles.handle1 = component_handle;
        pushUndoInstruction(undo_instruction)
        pushRedoInstruction(instruction)
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = source;
        undo_instruction->redo_instruction.u64 = 0;
        //set instruction data
        undo_instruction->type = bbI_unspawnAIComponent2;
        undo_instruction->data.three_handles.handle1 = component_handle;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        undo_instruction->type = bbI_unspawnAIComponent2;
        undo_instruction->data.three_handles.handle1 = component_handle;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_norewind)
    {

    }

    //take action



    bbCS_entity_setComponent(core,
                             ECS,
                             component->component.entity_handle,
                             component_handle,
                             bbECS_AI,
                             bbInstructionSource_internal,
                             no_handle);


    if (this != NULL) *this = component;


    return bbSuccess;

}



bbFlag bbCI_AI_setIdle(bbCore* core,
                             bbHandle AI_handle,
                             U64 current_time,
                             bbInstruction_source source,
                             bbHandle action)
{
    allocActiveInstruction(instruction)

    instruction->type = bbI_AI_setState;
    instruction->data.AI_state.AI_handle = AI_handle;
    instruction->data.AI_state.AI_state = bbAIState_Idle;
    instruction->data.AI_state.last_state_change = current_time;
    instruction->data.AI_state.last_attack = U64_MAX;
    instruction->source = source;
    instruction->redo_instruction = action;

    pushActiveInstruction(instruction)
    return bbSuccess;
}


bbFlag bbCI_AI_setApproaching(bbCore* core,
                             bbHandle AI_handle,
                             bbHandle target_handle,
                             U64 current_time,
                             bbInstruction_source source,
                             bbHandle action)
{
    allocActiveInstruction(instruction)

    instruction->type = bbI_AI_setState;
    instruction->data.AI_state.AI_handle = AI_handle;
    instruction->data.AI_state.AI_state = bbAIState_Approaching;
    instruction->data.AI_state.last_state_change = current_time;
    instruction->data.AI_state.target_handle = target_handle;
    instruction->data.AI_state.last_attack = U64_MAX;

    instruction->source = source;
    instruction->redo_instruction = action;

    pushActiveInstruction(instruction)
    return bbSuccess;
}


bbFlag bbCI_AI_setStriking(bbCore* core,
                             bbHandle AI_handle,
                             bbHandle target_handle,
                             U64 current_time,
                             bbInstruction_source source,
                             bbHandle action)
{
    allocActiveInstruction(instruction)

    instruction->type = bbI_AI_setState;
    instruction->data.AI_state.AI_handle = AI_handle;
    instruction->data.AI_state.AI_state = bbAIState_Striking;
    instruction->data.AI_state.last_state_change = current_time;
    instruction->data.AI_state.target_handle = target_handle;
    instruction->data.AI_state.last_attack = U64_MAX;

    instruction->source = source;
    instruction->redo_instruction = action;

    pushActiveInstruction(instruction)
    return bbSuccess;
}

bbFlag bbCI_AI_setMoving(bbCore* core,
                             bbHandle AI_handle,
                             U64 current_time,
                             bbInstruction_source source,
                             bbHandle action)
{
    allocActiveInstruction(instruction)

    instruction->type = bbI_AI_setState;
    instruction->data.AI_state.AI_handle = AI_handle;
    instruction->data.AI_state.AI_state = bbAIState_Moving;
    instruction->data.AI_state.last_state_change = current_time;
    instruction->data.AI_state.last_attack = U64_MAX;

    instruction->source = source;
    instruction->redo_instruction = action;

    pushActiveInstruction(instruction)
    return bbSuccess;
}


bbFlag bbCI_AI_setRecovering(bbCore* core,
                             bbHandle AI_handle,
                             U64 current_time,
                             bbInstruction_source source,
                             bbHandle action)
{
    allocActiveInstruction(instruction)

    instruction->type = bbI_AI_setState;
    instruction->data.AI_state.AI_handle = AI_handle;
    instruction->data.AI_state.AI_state = bbAIState_Recovering;
    instruction->data.AI_state.last_state_change = current_time;
    instruction->data.AI_state.last_attack = current_time;
    instruction->source = source;
    instruction->redo_instruction = action;

    pushActiveInstruction(instruction)
    return bbSuccess;
}

// bbFlag bbCI_AI_setState(bbCore* core,
//                              bbHandle AI_handle,
//                              U64 current_time,
//                              bbInstruction_source source,
//                              bbHandle action)
// {
//     bbInstruction* instruction;
//     bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);
//
//     instruction->type = bbI_AI_setState;
//     instruction->data.three_handles.handle1 = AI_handle;
//
//     instruction->data.three_handles.handle3.u64 = current_time;
//
//
//
//     instruction->source = source;
//     instruction->redo_instruction = action;
//
//     bbList_pushL(&core->do_stack, instruction);
//     return bbSuccess;
// }

bbFlag bbI_AI_setState_fn(bbCore* core, bbInstruction* instruction)
{
    bbHandle AI_handle = instruction->data.AI_state.AI_handle;
    bbAI_Component* component;
    bbVPool_lookup(home.ECS.AI_system.system.pool, (void**)&component, AI_handle);


    //bbHandle_getComponent(&home.ECS.AI_system.system,(bbComponent**)&component,AI_handle);

    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_AI_unsetState;

        undo_instruction->data.AI_state.AI_handle = AI_handle;
        undo_instruction->data.AI_state.AI_state = component->state;
        undo_instruction->data.AI_state.last_state_change = component->last_state_change;
        undo_instruction->data.AI_state.target_handle = component->target;
        undo_instruction->data.moveable_state.last_attack = component->last_attack;

        undo_instruction->source = instruction->source;
        //bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_AI_unsetState;

        undo_instruction->data.AI_state.AI_handle = AI_handle;
        undo_instruction->data.AI_state.AI_state = component->state;
        undo_instruction->data.AI_state.last_state_change = component->last_state_change;
        undo_instruction->data.AI_state.target_handle = component->target;
        undo_instruction->data.moveable_state.last_attack = component->last_attack;

        allocRedoInstruction(redo_instruction)
         *redo_instruction = *instruction;
        undo_instruction->redo_instruction = (bbHandle)redo_instruction_handle;
        pushRedoInstruction(redo_instruction)
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_action)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_AI_unsetState;
        undo_instruction->data.moveable_state.last_attack = component->last_attack;
        undo_instruction->data.AI_state.AI_handle = AI_handle;
        undo_instruction->data.AI_state.AI_state = component->state;
        undo_instruction->data.AI_state.last_state_change = component->last_state_change;
        undo_instruction->data.AI_state.target_handle = component->target;

        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)
    } //else source == no rewind

    if (instruction->data.moveable_state.last_attack < U64_MAX)
    {
        //bbNotImplemented() //is this correct?
        component->last_attack = instruction->data.AI_state.last_attack;
    }

    component->last_state_change = instruction->data.AI_state.last_state_change;
    component->state = instruction->data.AI_state.AI_state;

    switch (component->state)
    {
    case bbAIState_Idle:
        break;
    case bbAIState_Moving:
        break;
    case bbAIState_Approaching:
        component->target = instruction->data.AI_state.target_handle;
        break;
    case bbAIState_Striking:
        component->target = instruction->data.AI_state.target_handle;
        break;
    case bbAIState_Recovering:
        break;
    }

    return bbSuccess;
}

bbFlag bbI_AI_unsetState_fn(bbCore* core, bbInstruction* instruction)
{
    bbHandle AI_handle = instruction->data.AI_state.AI_handle;
    bbAI_Component* component;
    bbVPool_lookup(home.ECS.AI_system.system.pool, (void**)&component, AI_handle);

    component->state = instruction->data.AI_state.AI_state;
    component->last_state_change = instruction->data.AI_state.last_state_change;
    component->target = instruction->data.AI_state.target_handle;

    if (instruction->data.moveable_state.last_attack < U64_MAX)
    {
        //bbNotImplemented() //is this correct?
        component->last_attack = instruction->data.AI_state.last_attack;
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
    return bbSuccess;
}