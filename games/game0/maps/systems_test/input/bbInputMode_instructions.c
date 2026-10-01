#include "engine/userinterface/bbInputMode.h"
#include "engine/userinterface/bbInputMode_instructions.h"

#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction_operations.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbTerminal.h"
#include "games/game0/maps/systems_test/core/instructions.h"

bbFlag bbCI_bbInputModes_set(bbCore *core, bbInputModes *input_modes, char *string, bbInstruction_source source,
                             bbHandle action) {
    allocActiveInstruction(instruction)

    instruction->type = bbI_bbInputModes_set;

    bbHandle mode_handle;
    bbDictionary *dict = input_modes->dict;
    bbDictionary_lookup(dict, string, &mode_handle);
    instruction->data.three_handles.handle1.ptr = mode_handle.ptr;

    instruction->source = source;
    instruction->redo_instruction = action;

    pushActiveInstruction(instruction)

    return bbSuccess;
}

bbFlag bbCS_bbInputModes_set(bbCore *core, bbInputModes *input_modes, char *string, bbInstruction_source source,
                             bbHandle action) {
    bbNotImplemented()
}

bbFlag bbI_bbInputModes_set_fn(bbCore *core, bbInstruction *instruction) {
    bbInputModes *input_modes = &home.UI.input_modes;

    if (instruction->source == bbInstructionSource_internal) {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_bbInputModes_unset;
        undo_instruction->data.three_handles.handle1.ptr = input_modes->current_mode;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction.u64 = 0;
        pushUndoInstruction(undo_instruction)
    } else if (instruction->source == bbInstructionSource_input) {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_bbInputModes_unset;
        undo_instruction->data.three_handles.handle1.ptr = input_modes->current_mode;
        undo_instruction->source = instruction->source;
        allocRedoInstruction(redo_instruction)
        *redo_instruction = *instruction;
        undo_instruction->redo_instruction = (bbHandle) redo_instruction_handle;
        pushRedoInstruction(redo_instruction)
        pushUndoInstruction(undo_instruction)

        // bbHandle handle;
        // bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        // undo_instruction->redo_instruction = handle;
        // bbList_pushL(&core->undo_stack, (void*)undo_instruction);
    } else if (instruction->source == bbInstructionSource_action) {
        allocUndoInstruction(undo_instruction)
        undo_instruction->type = bbI_bbInputModes_unset;
        undo_instruction->data.three_handles.handle1.ptr = input_modes->current_mode;
        undo_instruction->source = instruction->source;
        undo_instruction->redo_instruction = instruction->redo_instruction;
        pushUndoInstruction(undo_instruction)
    } //else source == no rewind

    input_modes->current_mode = instruction->data.three_handles.handle1.ptr;
}

bbFlag bbI_bbInputModes_unset_fn(bbCore *core, bbInstruction *instruction) {
    //do side-effects
    bbInputModes *input_modes = &home.UI.input_modes;
    input_modes->current_mode = instruction->data.three_handles.handle1.ptr;

    if (instruction->source == bbInstructionSource_internal) {
        //bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input) {
        popRedoInstruction(redo_instruction, instruction)
        allocActiveInstruction(new_instruction)
        *new_instruction = redo_instruction;

        pushActiveInstruction(new_instruction)
        //bbInstruction* redo_instruction;
        //bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, instruction->redo_instruction);
        //bbList_pushL(&core->active_stack, redo_instruction);
        //bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action) {
        bbAction *redo_action;

        bbVPool_lookup(core->action_pool, (void **) &redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue, (void *) redo_action);
        //bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    bbAssert(0==1, "We should not get here\n");
}
