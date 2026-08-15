#include "engine/core/bbCore.h"
#include "engine/logic/bbFlag.h"
#include "engine/core/bbInstruction.h"

#include "bbAction.h"
#include "bbCoreInputs.h"
#include "engine/ECS/ECS.h"
#include "engine/test_string/bbTestString.h"

#ifdef DEFINE_SKELLYCHASE
#include "core/core_inputs.h"
#include "core/instructions.h"
#endif
#include "engine/data/bbHome.h"
#include "games/game0/maps/systems_test/core/spawn_entity.h"
#include "engine/logic/bbString.h"
#include "engine/network/bbNetworkApp.h"
#include "engine/userinterface/bbUI_Inbox.h"
#include "engine/data/bbHome.h"

extern char test_string[KEY_LENGTH];

U32 collision = 0;


/*

bbFlag bbInstruction_setString_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unsetString;
    bbStr_setStr(undo_instruction->data.key, test_string, KEY_LENGTH);
    undo_instruction->source = instruction->source;

    printf("    +old quote: %s, new quote %s, time = %lu\n",test_string,
        instruction->data.key, core->simulation_time);

    bbStr_setStr(test_string, instruction->data.key, KEY_LENGTH);


    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()
}


bbFlag bbInstruction_unsetString_fn(bbCore* core, bbInstruction* instruction)
{

    printf("-new quote %s, old quote: %s, time = %lu\n",
        instruction->data.key,test_string, core->simulation_time);

    bbStr_setStr(test_string,instruction->data.key, KEY_LENGTH);

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

}*/

bbFlag bbInstruction_setTime_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unsetTime;
    undo_instruction->data.u64 = core->simulation_time;
    undo_instruction->source = instruction->source;

    //bbDebug ("time was %lu, now is %lu, actual %lu\n", undo_instruction->data.unsigned_long,
    //    instruction->data.unsigned_long, core->actual_time);

    core->simulation_time = instruction->data.u64 ;

#ifdef DEFINE_TEST_ECS
    bbDebug("+time = %lu\n", core->simulation_time);
#endif
    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);
        return bbSuccess;
    }
    bbNotHere()


}


bbFlag bbInstruction_unsetTime_fn(bbCore* core, bbInstruction* instruction)
{
    //bbDebug ("unset time was %lu, now is %lu, actual is %lu\n", core->simulation_time,
    //instruction->data.unsigned_long, core->actual_time);

    core->simulation_time = instruction->data.u64 ;



#ifdef DEFINE_TEST_ECS
    bbDebug("-time = %lu\n", core->simulation_time);
#endif
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
        //TODO place instruction->redo_instruction into core->action_queue
        bbAction* redo_action;
        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    bbNotHere()

}

/*
bbFlag bbInstruction_netpauseButton_fn(bbCore* core, bbInstruction* instruction)
{
    //TODO virtual function / callback
    //bbNetworkApp_netpauseButton(&home.network, instruction->data.key);
    return bbSuccess;
}

*/
/*
bbFlag bbInstruction_unfreezeButton_fn(bbCore* core, bbInstruction* instruction)
{

    //TODO virtual function / callback
    bbDebug("unfreeze button %s\n", instruction->data.key);

    //bbUI_Inbox_UnpressButton2(&home.UI.inbox, instruction->data.key);

    return bbSuccess;

}
*/
