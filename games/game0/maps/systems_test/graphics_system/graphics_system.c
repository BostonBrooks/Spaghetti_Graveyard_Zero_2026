#include "core/instructions.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbList.h"
#include "engine/userinterface/bbUI_Inbox.h"

bbFlag bbCoreInput_spawnGraphicsComponent(bbCore* core,
                                          bbMapCoords MC,
                                          bbHandle entity,
                                          bbHandle moveable,
                                          bbInstruction_source source,
                                          bbHandle action)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**)&instruction);
    instruction->type = bbInstruction_spawnGraphicsComponent;
    instruction->data.agent_MC.handle1 = entity;
    instruction->data.agent_MC.handle2 = moveable;
    instruction->data.agent_MC.coords = MC;

    instruction->source = source;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

bbFlag bbInstruction_spawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction)
{

    I32 skelly_int = bbViewportSpawner_lookupInt(&home.viewport_app.viewport_spawner,
                            "SKELLY");

    bbUI_Inbox_NewDrawable(&home.UI.inbox,
                           skelly_int,
                           instruction->data.agent_MC.coords,
                           instruction->data.agent_MC.handle1,
                           instruction->data.agent_MC.handle2);

    if (instruction->source == bbInstructionSource_internal)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbInstruction_unspawnGraphicsComponent;
        undo_instruction->data.agent_MC.handle1 = instruction->data.agent_MC.handle1;
        undo_instruction->data.agent_MC.handle2 = instruction->data.agent_MC.handle2;

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

        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }

    bbGraphicsSystem* graphics = (bbGraphicsSystem*)core->ECS->systems[bbECS_Graphics];
    bbGraphicsComponent* component;
    bbHandle component_handle;

    bbVPool_alloc2(graphics->system.pool, (void**) &component, &component_handle);

    bbHandle entity_handle = instruction->data.three_handles.handle1;
    component->component.entity_handle = entity_handle;

    bbCoreInput_entity_setComponent(core, core->ECS, entity_handle, component_handle, bbECS_Graphics,
                                    bbInstructionSource_internal, no_handle);



    return bbSuccess;
}
