
#include "core/instructions.h"
#include "engine/core/bbAction.h"
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