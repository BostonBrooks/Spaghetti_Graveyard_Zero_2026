
#include "core/instructions.h"
#include "engine/core/bbAction.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/AI_system/bbAI_System.h"
#include "games/game0/maps/ECS_test/virtual_instructions/instructions.h"

bbFlag bbCS_updateAI(bbCore* core, bbInstruction_source source, bbHandle action)
{

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        bbInstruction* instruction;
        bbHandle instruction_handle;
        bbFlag flag = bbList_alloc2(&core->do_stack,(void**)&instruction, &instruction_handle);

        //set input instruction data
        instruction->type = bbInstruction_updateAI;

        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction_handle;

        //set instruction data
        undo_instruction->type = bbInstruction_unupdateAI;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = source;

        //set instruction data
        undo_instruction->type = bbInstruction_unupdateAI;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        undo_instruction->type = bbInstruction_unupdateAI;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_norewind)
    {

    }



    bbAI_System_update((bbAI_System*) core->ECS->systems[bbECS_AI]);
}


bbFlag bbCI_updateAI(bbCore* core, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbInstruction_updateAI;
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}



bbFlag bbI_updateAI_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbInstruction_unupdateAI;
        undo_instruction->source = instruction->source;
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbInstruction_unupdateAI;
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
        undo_instruction->type = bbInstruction_unupdateAI;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    } //else source == no rewind


    bbAI_System_update((bbAI_System*) core->ECS->systems[bbECS_AI]);

    return bbSuccess;
}
bbFlag bbI_unupdateAI_fn(bbCore* core, bbInstruction* instruction)
{


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

    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);
    instruction->type = bbI_spawnAIComponent2;
    instruction->data.three_handles.handle1 = entity;
    instruction->data.three_handles.handle2.bloated.index = update;
    instruction->data.three_handles.handle2.bloated.collision = command;
    instruction->source = source;
    instruction->redo_instruction = action;
    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}


bbFlag bbI_spawnAIComponent2_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_unspawnAIComponent2;
        undo_instruction->data.three_handles.handle1 = instruction->data.
            three_handles.handle1;
        undo_instruction->source = instruction->source;
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbI_unspawnAIComponent2;
        undo_instruction->data.three_handles.handle1 = instruction->data.
            three_handles.handle1;
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
        undo_instruction->type = bbI_unspawnAIComponent2;
        undo_instruction->data.three_handles.handle1 = instruction->data.
            three_handles.handle1;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    } //else source == no rewind


    bbAI_Component* component;
    bbHandle component_handle;

    bbList_alloc2(&home.ECS.AI_system.list,(void**)&component, &component_handle);
    component->ftable.update = instruction->data.three_handles.handle2.bloated.index;
    component->ftable.command = instruction->data.three_handles.handle2.bloated.collision;
    component->state = 0;
    component->component.entity_handle = instruction->data.three_handles.handle1;

    bbCS_entity_setComponent(core,
                             home.ECS.ECS,
                             component->component.entity_handle,
                             component_handle,
                             bbECS_AI,
                             bbInstructionSource_internal,
                             no_handle);

    bbList_pushL(&home.ECS.AI_system.list,component);
    return bbSuccess;
}

bbFlag bbI_unspawnAIComponent2_fn(bbCore* core, bbInstruction* instruction)
{
    bbNotHere()
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
    //Post undo instruction

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        bbInstruction* instruction;
        bbHandle instruction_handle;
        bbFlag flag = bbList_alloc2(&core->do_stack,(void**)&instruction, &instruction_handle);

        //set input instruction data
        instruction->type = bbI_spawnAIComponent2;
        instruction->data.three_handles.handle1 = entity;
        instruction->data.three_handles.handle2.bloated.index = update;
        instruction->data.three_handles.handle2.bloated.collision = command;

        instruction->source = source;
        instruction->redo_instruction = action;
        //bbStr_setStr(instruction->data.key, string, KEY_LENGTH);

        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction_handle;

        //set instruction data
        undo_instruction->type = bbI_unspawnAIComponent2;
        undo_instruction->data.three_handles.handle1 = entity;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->source = source;
        undo_instruction->redo_instruction.u64 = 0;
        //set instruction data
        undo_instruction->type = bbI_unspawnAIComponent2;
        undo_instruction->data.three_handles.handle1 = entity;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        undo_instruction->type = bbI_unspawnAIComponent2;
        undo_instruction->data.three_handles.handle1 = entity;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
    } else if (source == bbInstructionSource_norewind)
    {

    }

    //take action

    bbAI_Component* component;
    bbHandle component_handle;

    bbAI_System* AI_System = (bbAI_System*)ECS->systems[bbECS_AI];
    bbList_alloc2(&AI_System->list,(void**)&component, &component_handle);
    component->ftable.command = command;
    component->ftable.update = update;
    component->state = 0;
    component->component.entity_handle = entity;

    bbCS_entity_setComponent(core,
                             ECS,
                             component->component.entity_handle,
                             component_handle,
                             bbECS_AI,
                             bbInstructionSource_internal,
                             no_handle);

    bbList_pushL(&AI_System->list,component);

    if (this != NULL) *this = component;


    return bbSuccess;

}


