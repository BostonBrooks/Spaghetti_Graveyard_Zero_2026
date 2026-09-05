#include "core/instructions.h"
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


    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**)&instruction);
    instruction->type = bbInstruction_spawnGraphicsComponent;
    instruction->data.agent_MC.type = type_int;
    instruction->data.agent_MC.handle1 = entity;
    instruction->data.agent_MC.handle2 = moveable;
    instruction->data.agent_MC.coords = MC;
    instruction->data.agent_MC.drawable_state = drawable_state;

    instruction->source = source;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction);
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
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbInstruction_unspawnGraphicsComponent;
        undo_instruction->data.agent_MC.handle1 = instruction->data.agent_MC.handle1;
        undo_instruction->data.agent_MC.handle2 = instruction->data.agent_MC.handle2;
        undo_instruction->data.agent_MC.type = instruction->data.agent_MC.type;

        undo_instruction->source = instruction->source;
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbInstruction_unspawnGraphicsComponent;
        undo_instruction->data.agent_MC.handle1 = instruction->data.agent_MC.handle1;
        undo_instruction->data.agent_MC.handle2 = instruction->data.agent_MC.handle2;
        undo_instruction->data.agent_MC.type = instruction->data.agent_MC.type;

        undo_instruction->source = instruction->source;
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (instruction->source == bbInstructionSource_action)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbInstruction_unspawnGraphicsComponent;
        undo_instruction->data.agent_MC.handle1 = instruction->data.agent_MC.handle1;
        undo_instruction->data.agent_MC.handle2 = instruction->data.agent_MC.handle2;
        undo_instruction->data.agent_MC.type = instruction->data.agent_MC.type;

        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
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

    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;
        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        printf("collision = %d ", redo_action->header.collision);
    }
    bbHandle entity_handle = instruction->data.agent_MC.handle1;
    bbHandle moveable_handle = instruction->data.agent_MC.handle2;
    bbUI_Inbox_DeleteUnit(&home.UI.inbox,entity_handle,moveable_handle);

    //TODO At some point we'll want to de-allocate component from graphics->system.pool

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* redo_instruction;
        bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        bbList_pushL(&core->do_stack, redo_instruction);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
    }

}