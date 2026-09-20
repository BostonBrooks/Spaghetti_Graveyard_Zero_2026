#include "core/instructions.h"
#include "engine/core/bbInstruction_operations.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbList.h"
#include "engine/userinterface/bbUI_Inbox.h"

bbFlag bbCoreInput_spawnGraphicsComponent(bbCore* core,
                                          char* type,
                                          bbMapCoords MC,
                                          I32 drawable_state,
                                          bbHandle entity,
                                          bbHandle moveable,
                                          bbInstruction_source source,
                                          bbHandle action)
{
    U32 type_int =  bbViewportSpawner_lookupInt(&home.viewport_app.viewport_spawner,
                                type);



    allocActiveInstruction(instruction)
    instruction->type = bbInstruction_spawnGraphicsComponent;
    instruction->data.agent_MC.type = type_int;
    instruction->data.agent_MC.handle1 = entity;
    instruction->data.agent_MC.handle2 = moveable;
    instruction->data.agent_MC.coords = MC;
    instruction->data.agent_MC.drawable_state = drawable_state;

    instruction->source = source;
    instruction->redo_instruction = action;
    pushActiveInstruction(instruction)
    return bbSuccess;
}

bbFlag bbInstruction_spawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction)
{




    bbUI_Inbox_NewDrawable(&home.UI.inbox,
                           instruction->data.agent_MC.type,
                           instruction->data.agent_MC.coords,
                           instruction->data.agent_MC.handle1,
                           instruction->data.agent_MC.handle2);


    bbUI_Inbox_SetEntityState(&home.UI.inbox,instruction->data.agent_MC.handle1, instruction->data.agent_MC.drawable_state);

    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbInstruction_unspawnGraphicsComponent;
        undo_instruction->data.agent_MC.handle1 = instruction->data.agent_MC.handle1;
        undo_instruction->data.agent_MC.handle2 = instruction->data.agent_MC.handle2;
        undo_instruction->data.agent_MC.type = instruction->data.agent_MC.type;

        undo_instruction->source = instruction->source;
        //bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbInstruction_unspawnGraphicsComponent;
        undo_instruction->data.agent_MC.handle1 = instruction->data.agent_MC.handle1;
        undo_instruction->data.agent_MC.handle2 = instruction->data.agent_MC.handle2;
        undo_instruction->data.agent_MC.type = instruction->data.agent_MC.type;

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
        undo_instruction->type = bbInstruction_unspawnGraphicsComponent;
        undo_instruction->data.agent_MC.handle1 = instruction->data.agent_MC.handle1;
        undo_instruction->data.agent_MC.handle2 = instruction->data.agent_MC.handle2;
        undo_instruction->data.agent_MC.type = instruction->data.agent_MC.type;

        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)
    }

    bbGraphicsSystem* graphics = (bbGraphicsSystem*)core->ECS->systems[bbECS_Graphics];
    bbGraphicsComponent* component;
    bbHandle component_handle;

    bbVPool_alloc2(graphics->system.pool, (void**) &component, &component_handle);

    bbHandle entity_handle = instruction->data.agent_MC.handle1;
    component->component.entity_handle = entity_handle;

    bbCoreInput_entity_setComponent(core, core->ECS, entity_handle, component_handle, bbECS_Graphics,
                                    bbInstructionSource_internal, no_handle);



    return bbSuccess;
}

bbFlag bbInstruction_unspawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction)
{


    bbHandle entity_handle = instruction->data.agent_MC.handle1;
    bbHandle moveable_handle = instruction->data.agent_MC.handle2;
    bbUI_Inbox_DeleteUnit(&home.UI.inbox,entity_handle,moveable_handle);

    //TODO At some point we'll want to de-allocate component from graphics->system.pool

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
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        //bbVPool_free(core->instruction_pool, (void*)instruction);
    }

}