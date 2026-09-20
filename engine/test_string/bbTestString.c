#include "engine/test_string/bbTestString.h"

#include "engine/core/bbAction.h"
#include "engine/core/bbInstruction.h"
#include "engine/logic/bbString.h"
#include "engine/core/bbInstruction_operations.h"

bbFlag bbI_setString_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unsetString;
        bbStr_setStr(undo_instruction->data.key, test_string, KEY_LENGTH);
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_input)
    {

        allocUndoInstruction(undo_instruction);
        undo_instruction->type = bbI_unsetString;
        bbStr_setStr(undo_instruction->data.key, test_string, KEY_LENGTH);
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
        undo_instruction->type = bbI_unsetString;
        bbStr_setStr(undo_instruction->data.key, test_string, KEY_LENGTH);
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = (bbHandle)instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)

        bbAction* action;
        bbVPool_lookup(core->action_queue.pool, (void**)&action, instruction->redo_instruction);
        printf("collision = %d ", action->header.collision);
    } //else source == no rewind

    printf("    +old quote: %s, new quote %s, simulation time = %lu, actual time = %lu\n",test_string,
        instruction->data.key, core->simulation_time,core->actual_time);

    bbStr_setStr(test_string, instruction->data.key, KEY_LENGTH);

    return bbSuccess;
}
bbFlag bbI_unsetString_fn(bbCore* core, bbInstruction* instruction)
{
//bbHere()
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;
        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        printf("collision = %d ", redo_action->header.collision);
    }

    printf("-new quote %s, old quote: %s, simulation time = %lu, actual time = %lu\n",
        instruction->data.key,test_string, core->simulation_time, core->actual_time);

    bbStr_setStr(test_string,instruction->data.key, KEY_LENGTH);

    if (instruction->source == bbInstructionSource_internal)
    {
//bbHere()
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        popRedoInstruction(redo_instruction,instruction)
        allocActiveInstruction(new_instruction)
        *new_instruction = redo_instruction;
        pushActiveInstruction(new_instruction)
//bbHere()
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        bbAction* redo_action;

        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);




        //bbHere()
        return bbSuccess;
    }
    bbAssert(0==1, "We should not get here\n");

}

bbFlag bbCI_setString(bbCore* core, char* string, bbInstruction_source source, bbHandle action)
{

    allocActiveInstruction(instruction)

    instruction->type = bbI_setString;
    bbStr_setStr(instruction->data.key, string, KEY_LENGTH);
    instruction->source = source;
    instruction->redo_instruction = action;

    pushActiveInstruction(instruction)
    return bbSuccess;
}
bbFlag bbCS_setString(bbCore* core, char* string, bbInstruction_source source, bbHandle action)
{


    if (source == bbInstructionSource_input)
    {
        //create input instruction
        allocRedoInstruction(instruction)
        instruction->source = source;
        //set input instruction data
        instruction->type = bbI_setString;
        bbStr_setStr(instruction->data.key, string, KEY_LENGTH);
        pushRedoInstruction(instruction)
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_unsetString;
        undo_instruction->source = source;
        undo_instruction->redo_instruction = (bbHandle)instruction_handle;

        //set instruction data
        bbStr_setStr(undo_instruction->data.key, test_string, KEY_LENGTH);
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction

        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_unsetString;
        undo_instruction->source = source;

        //set instruction data
        bbStr_setStr(undo_instruction->data.key, test_string, KEY_LENGTH);
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_action)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_unsetString;
        undo_instruction->redo_instruction = action;
        undo_instruction->source = source;

        //Set instruction data
        bbStr_setStr(undo_instruction->data.key, test_string, KEY_LENGTH);
        pushUndoInstruction(undo_instruction)
    } else if (source == bbInstructionSource_norewind)
    {

    }

    //take action

    printf("    +old quote: %s, new quote %s, time = %llu\n",test_string,
    string, core->simulation_time);

    bbStr_setStr(test_string, string, KEY_LENGTH);

    return bbSuccess;
}


bbFlag bbI_doNothing_fn(bbCore* core, bbInstruction* instruction)
{
    if (instruction->source == bbInstructionSource_internal)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_undoNothing;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    }
    else if (instruction->source == bbInstructionSource_input)
    {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_undoNothing;
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
        undo_instruction->type = bbI_undoNothing;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)

    } //else source == no rewind



    return bbSuccess;
}
bbFlag bbI_undoNothing_fn(bbCore* core, bbInstruction* instruction)
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

bbFlag bbCI_doNothing(bbCore* core,  bbInstruction_source source, bbHandle action)
{
    allocActiveInstruction(instruction)
    instruction->type = bbI_doNothing;
    instruction->source = source;
    instruction->redo_instruction = action;

    pushActiveInstruction(instruction)
    return bbSuccess;
}
bbFlag bbCS_doNothing(bbCore* core,  bbInstruction_source source, bbHandle action)
{
    //Post undo instruction

    if (source == bbInstructionSource_input)
    {
        //create input instruction
        allocRedoInstruction(instruction)
        instruction->source = source;
        //set input instruction data
        instruction->type = bbI_doNothing;

        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = source;
        undo_instruction->redo_instruction = (bbHandle)instruction_handle;

        //set instruction data
        undo_instruction->type = bbI_undoNothing;
        pushUndoInstruction(undo_instruction)
        pushRedoInstruction(instruction)
    } else if (source == bbInstructionSource_internal)
    {
        //create undo instruction
        allocUndoInstruction(undo_instruction)
        undo_instruction->source = source;

        //set instruction data
        undo_instruction->type = bbI_undoNothing;
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


    return bbSuccess;
}
