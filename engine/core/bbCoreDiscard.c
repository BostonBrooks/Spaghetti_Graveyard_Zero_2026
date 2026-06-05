#include "engine/core/bbCoreDiscard.h"

#include "engine/data/bbHome.h"

#ifdef DEFINE_SKELLYCHASE
#include "games/game0/maps/skellychase/core/instructions.h"
#endif

bbFlag bbCoreDiscard(bbCore* core, U64 time)
{
    bbInstruction *undo_instruction, *redo_instruction;
    bbAction* action;
    bbFlag flag;

    bbHere()
    while (1)
    {
        bbHere()
        flag = bbList_peakR(&core->undo_stack, (void**)&undo_instruction);

        bbHere()

        if (flag != bbSuccess) return bbSuccess;

        bbHere()

        if (undo_instruction->type == bbInstruction_unsetTime)
        {
            if (undo_instruction->data.u64 >= time)
            {
                return bbSuccess;
            }
        }


        bbHere()
        flag = bbList_popR(&core->undo_stack, (void**)&undo_instruction);

#ifdef DEFINE_SKELLYCHASE
        if (undo_instruction->type == bbVInstruction_undeleteEntity)
        {
            bbHere()
            bbUI_Inbox_DeleteUnit2(&home.UI.inbox, undo_instruction->data.entity.unit);

        }

#endif
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

        //if (undo_instruction->type == bbInstruction_unupdateMovables)
        //{
        //    bbMovables_snapshot* snapshot;
       //     bbVPool_lookup(home.agents_app.movables.snapshots, (void**)&snapshot, undo_instruction->snapshot);
        //    bbVPool_free(home.agents_app.movables.snapshots, snapshot);
        //}

        bbVPool_free(core->instruction_pool, undo_instruction);
    }

    return bbSuccess;
}
