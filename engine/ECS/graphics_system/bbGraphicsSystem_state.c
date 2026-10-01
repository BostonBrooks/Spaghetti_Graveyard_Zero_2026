


#include "bbGraphicsSystem.h"
#include "bbGraphicsSystem_data.h"
#include "engine/core/bbInstruction.h"
#include "engine/core/bbInstruction_operations.h"
#include "engine/ECS/bbECS_instructions.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/ECS/graphics_system/bbGraphicsSystem_state.h"

#include "engine/userinterface/bbUI_Inbox.h"
// typedef struct {
//     bbHandle entity_handle;
//     bbMapCoords MC;
//     bbTime last_state_change;
//     bbTime last_wander_time;
//     U64 random_seed;
//     I32 drawable_state;
// } bbGraphicsComponent_data;
bbFlag bbGraphics_updateState(bbGraphicsComponent_data* new, bbGraphicsComponent_data* old, I32 state, bbTime time) {

    U64 time_diff = time - old->last_state_change;

    new->entity_handle = old->entity_handle;
    new->MC = old->MC;
    new->last_state_change = time;

    if (old->drawable_state == bbGraphicsState_moving)
    {
        new->last_wander_time = old->last_wander_time + time_diff;
    } else
    {
        new->last_wander_time = old->last_wander_time;
    }
    new->random_seed = old->random_seed;
    new->drawable_state = state;


    return bbSuccess;
}

bbFlag bbCS_Graphics_setState(bbCore* core,
                              bbGraphicsComponent* component,
                              bbHandle entity_handle,
                              I32 state,
                              bbTime time,
                              bbInstruction_source source,
                              bbHandle action)
{
    bbGraphicsComponent* component2;
    if (component == NULL)
    {
        bbHandle_mapComponent(core->ECS,bbECS_ECS,entity_handle,bbECS_Graphics,NULL,(bbComponent**)&component2);
    }
    else
    {
        component2 = component;
    }


    bbGraphicsComponent_data data;

    bbGraphics_updateState(&data , &component2->data, state, time);

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        allocRedoInstruction(instruction)
        instruction->type = bbI_Graphics_setState;
        instruction->data.graphics = data;
        // bbDebug("attempting to set state: %d\n",instruction->data.graphics.drawable_state);
        instruction->source = source;
        instruction->redo_instruction = action;

        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = source;
        undo_instruction->redo_instruction = (bbHandle)instruction_handle;
        undo_instruction->data.graphics = component2->data;
        //set instruction data
        undo_instruction->type = bbI_Graphics_unsetState;
        pushUndoInstruction(undo_instruction)
        pushRedoInstruction(instruction)
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = source;
        undo_instruction->data.graphics = component2->data;
        //set instruction data
        undo_instruction->type = bbI_Graphics_unsetState;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        undo_instruction->type = bbI_undoNothing;
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_norewind)
    {

    }
    bbGraphicsSystem* system = (bbGraphicsSystem*)core->ECS->systems[bbECS_Graphics];
    bbUI_Inbox_SetEntityState2(system->inbox, &data);


    component->data = data;
    return bbSuccess;
}

bbFlag bbCI_Graphics_setState(bbCore* core,
                              bbGraphicsComponent* component,
                              bbHandle entity_handle,
                              I32 state,
                              bbTime time,
                              bbInstruction_source source,
                              bbHandle action)
{
    bbGraphicsComponent* component2;
    if (component == NULL)
    {
        bbHandle_mapComponent(core->ECS,bbECS_ECS,entity_handle,bbECS_Graphics,NULL,(bbComponent**)&component2);
    }
    else
    {
        component2 = component;
    }

    //bbDebug("attempting to set state: %d\n", state);

    allocActiveInstruction(instruction)
    instruction->type = bbI_Graphics_setState;
    bbGraphics_updateState(&instruction->data.graphics , &component2->data, state, time);
   // bbDebug("attempting to set state: %d\n",instruction->data.graphics.drawable_state);
    instruction->source = source;
    instruction->redo_instruction = action;
    pushActiveInstruction(instruction)
    return bbSuccess;

}


bbFlag bbI_Graphics_setState_fn(bbCore* core, bbInstruction* instruction)
{

    bbGraphicsComponent* component;
    bbHandle_mapComponent(core->ECS,bbECS_ECS,instruction->data.graphics.entity_handle,
        bbECS_Graphics,NULL,(bbComponent**)&component);

    bbAssert(component != NULL, "trying to update a null graphics component\n");
   // bbDebug("attempting to set state: %d\n", instruction->data.graphics.drawable_state);
    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_Graphics_unsetState;
        undo_instruction->data.graphics = component->data;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_input)
    {

        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_Graphics_unsetState;
        undo_instruction->data.graphics = component->data;
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
        undo_instruction->type = bbI_Graphics_unsetState;
        undo_instruction->data.graphics = component->data;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = (bbHandle)instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)

        bbAction* action;
        bbVPool_lookup(core->action_queue.pool, (void**)&action, instruction->redo_instruction);
        printf("collision = %d ", action->header.collision);
    } //else source == no rewind

    bbGraphicsSystem* system = (bbGraphicsSystem*)core->ECS->systems[bbECS_Graphics];
    bbUI_Inbox_SetEntityState2(system->inbox, &instruction->data.graphics);


    component->data = instruction->data.graphics;



    return bbSuccess;
}
bbFlag bbI_Graphics_unsetState_fn(bbCore* core, bbInstruction* instruction)
{


    bbGraphicsComponent* component;
    bbHandle_mapComponent(core->ECS,bbECS_ECS,instruction->data.graphics.entity_handle,
        bbECS_Graphics,NULL,(bbComponent**)&component);

    component->data = instruction->data.graphics;


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
}
