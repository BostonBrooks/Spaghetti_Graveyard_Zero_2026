#include "engine/core/bbCoreDiscard.h"

#include "engine/data/bbHome.h"


#ifdef DEFINE_TEST_SYSTEM
#include "games/game0/maps/systems_test/core/instructions.h"
#endif

bbFlag bbCoreDiscard(bbCore* core, bbTime time)
{
    bbInstruction *undo_instruction, *redo_instruction;
    bbAction* action;
    bbFlag flag;

    while (1)
    {
        flag = bbList_peakR(&core->undo_stack, (void**)&undo_instruction);


        if (flag != bbSuccess) return bbSuccess;


        if (undo_instruction->type == bbInstruction_unsetTime)
        {
            if (undo_instruction->data.u64 >= time)
            {
                return bbSuccess;
            }
        }


        flag = bbList_popR(&core->undo_stack, (void**)&undo_instruction);



        if (undo_instruction->source == bbInstructionSource_action)
        {

            bbAssert(undo_instruction->redo_instruction.ptr != NULL, "redo action not found\n");

            bbVPool_lookup(core->action_pool, (void**)&action, undo_instruction->redo_instruction);


            bbAssert(action != NULL, "redo action not found type = %d\n", undo_instruction->type);

            bbVPool_free(core->action_pool, action);

        }

        if (undo_instruction->source == bbInstructionSource_input)
        {
            bbDebug("undo instruction = %d\n", undo_instruction->type);
            fflush(stdout);
            bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, undo_instruction->redo_instruction);
            bbAssert(undo_instruction->redo_instruction.ptr != NULL, "redo action not found\n");

            bbVPool_free(core->instruction_pool,redo_instruction);

        }

        if (undo_instruction->type > bbInstruction_numTypes)
        {
            bbInstruction_fn* function = core->discard_functions[undo_instruction->type - bbInstruction_numTypes];
            if (function != NULL)
                function(core,undo_instruction);
        }


        bbVPool_free(core->instruction_pool, undo_instruction);
    }

    return bbSuccess;
}

bbFlag bbCoreDiscardSegmented(bbCore* core, bbTime time)
{
    bbInstruction *undo_instruction, *redo_instruction;
    bbAction* action;
    bbFlag flag;

    while (1)
    {
        //flag = bbList_peakR(&core->undo_stack, (void**)&undo_instruction);
        flag = bbInstruction_deque_peakBack(&core->undo_instructions, &undo_instruction);


        if (flag != bbSuccess) return bbSuccess;


        if (undo_instruction->type == bbInstruction_unsetTime)
        {
            if (undo_instruction->data.u64 >= time)
            {
                return bbSuccess;
            }
        }


        //flag = bbList_popR(&core->undo_stack, (void**)&undo_instruction);
        //flag = bbInstruction_deque_popBack(&core->undo_instructions, &undo_instruction);



        if (undo_instruction->source == bbInstructionSource_action)
        {

            bbAssert(undo_instruction->redo_instruction.ptr != NULL, "redo action not found\n");

            bbVPool_lookup(core->action_pool, (void**)&action, undo_instruction->redo_instruction);


            bbAssert(action != NULL, "redo action not found type = %d\n", undo_instruction->type);

            bbVPool_free(core->action_pool, action);

        }

        if (undo_instruction->source == bbInstructionSource_input)
        {
            bbInstruction* redo_instruction2 = undo_instruction->redo_instruction.ptr;
            //bbDebug("undo instruction = %d, redo instruction = %d\n",
            //                 undo_instruction->type, redo_instruction2->type);
            //fflush(stdout);


            bbFlag flag2 = bbInstruction_deque_peakBack(&core->redo_instructions, &redo_instruction);
            //bbFlag_print(flag2);

            //TODO peak_back and pop_back return different elements?

            //bbDebug("undo instruction = %p, redo instruction = %p\n",
            //                 undo_instruction, redo_instruction2);

            bbAssert(redo_instruction2 == redo_instruction, "redo instruction mismatch, types %d, %d\n",
                     redo_instruction2->type,redo_instruction2->type);
            bbAssert(redo_instruction2 != NULL, "redo action not found\n");
            //bbVPool_lookup(core->instruction_pool, (void**)&redo_instruction, undo_instruction->redo_instruction);


            bbInstruction_deque_popBack(&core->redo_instructions, &redo_instruction);
        }

        if (undo_instruction->type > bbInstruction_numTypes)
        {
            bbInstruction_fn* function = core->discard_functions[undo_instruction->type - bbInstruction_numTypes];
            if (function != NULL)
                function(core,undo_instruction);
        }



        flag = bbInstruction_deque_popBack(&core->undo_instructions, &undo_instruction);
    }

    return bbSuccess;
}
