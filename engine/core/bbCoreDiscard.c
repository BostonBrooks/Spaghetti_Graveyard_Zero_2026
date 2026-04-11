#include "engine/core/bbCoreDiscard.h"


bbFlag bbCoreDiscard(bbCore* core, U64 time)
{
    bbInstruction *undo_instruction, *redo_instruction;
    bbAction* action;
    bbFlag flag;

    while (1)
    {
        flag = bbList_peakR(&core->undo_stack, (void**)&undo_instruction);


        if (flag != bbSuccess) return bbSuccess;

        bbDebug("undo instruction type %d\n", undo_instruction->type);

        if (undo_instruction->type == bbInstruction_unsetTime)
        {
            if (undo_instruction->data.unsigned_long >= time)
            {
                return bbSuccess;
            }
        }
        flag = bbList_popR(&core->undo_stack, (void**)&undo_instruction);

        if (undo_instruction->source == bbInstructionSource_action)
        {
            bbVPool_lookup(core->action_pool, (void**)&action, undo_instruction->redo_instruction);
            bbVPool_free(core->action_pool, action);

        }

        if (undo_instruction->source == bbInstructionSource_input)
        {
            bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, undo_instruction->redo_instruction);
            bbVPool_free(core->instruction_pool,redo_instruction);

        }

        bbVPool_free(core->instruction_pool, undo_instruction);
    }

    return bbSuccess;
}