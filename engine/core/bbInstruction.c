#include "engine/core/bbInstruction.h"

#include "bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbString.h"
#include "engine/logic/bbTerminal.h"
#include "engine/network/bbNetworkApp.h"
#include "engine/userinterface/bbUI_Inbox.h"


bbFlag bbInstruction_printInteger_fn(bbCore* core, bbInstruction* instruction)
{
    bbDebug("print integer: %d\n", instruction->data.integer.integer);
    return bbSuccess;
}
bbFlag bbInstruction_unprintInteger_fn(bbCore* core, bbInstruction* instruction)
{
    bbDebug("unprint integer: %d\n", instruction->data.integer.integer);
    return bbSuccess;
}

bbFlag bbInstruction_printString_fn(bbCore* core, bbInstruction* instruction)
{
    bbDebug("print string: %s\n", instruction->data.string.string);

    return bbSuccess;
}
bbFlag bbInstruction_unprintString_fn(bbCore* core, bbInstruction* instruction)
{
    bbDebug("unprint string: %s\n", instruction->data.string.string);
    return bbSuccess;
}


bbFlag bbInstruction_unfreezeButton_fn(bbCore* core, bbInstruction* instruction)
{
    bbUI_Inbox_UnpressButton(&home.UI.inbox);
    return bbSuccess;
}

bbFlag bbInstruction_unfreezeButton2_fn(bbCore* core, bbInstruction* instruction)
{
    bbUI_Inbox_UnpressButton2(&home.UI.inbox, instruction->data.string.string);
    return bbSuccess;
}


bbFlag bbInstruction_netsendButton_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_netsendButton(&home.network, instruction->data.string.string);
    return bbSuccess;
}


bbFlag bbInstruction_netcodeButton_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_netcodeButton(&home.network, instruction->data.string.string, instruction->act_time);
    return bbSuccess;
}

bbFlag bbInstruction_loopAction_fn(bbCore* core, bbInstruction* instruction)
{
    bbDebug("Loop instruction at time = %lu\n", instruction->act_time);
    bbAction_loop(core,
                  0,
                  0,
                  0,
                  instruction->act_time + 120,
                  instruction->data.string.string);
    return bbSuccess;

}

///(5) instruction definition
bbFlag bbInstruction_setQuote_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unsetQuote;
    bbStr_setStr(undo_instruction->data.string.string, home.core.quote, KEY_LENGTH);
    undo_instruction->source = instruction->source;

    printf("    +old quote: %s, new quote %s\n",home.core.quote, instruction->data.string.string);

    bbStr_setStr(home.core.quote, instruction->data.string.string, KEY_LENGTH);


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
    bbAssert(0==1, "We should not get here\n");
}

///(7) un-instruction definition
bbFlag bbInstruction_unsetQuote_fn(bbCore* core, bbInstruction* instruction)
{

    printf("-old quote: %s, new quote %s\n",home.core.quote, instruction->data.string.string);

    bbStr_setStr(home.core.quote,instruction->data.string.string, KEY_LENGTH);

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


bbFlag bbInstruction_setTime_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unsetTime;
    undo_instruction->data.integer.integer = core->simulation_time;
    undo_instruction->source = instruction->source;


    core->simulation_time = instruction->data.integer.integer;

    printf("                                         +time = %lu\n", core->simulation_time);
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
    core->simulation_time = instruction->data.integer.integer;

    printf("                                         -time = %lu\n", core->simulation_time);
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



bbFlag bbInstruction_checkActions_fn(bbCore* core, bbInstruction* instruction)
{
    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_uncheckActions;
    undo_instruction->source = instruction->source;

    bbActions_react(&home.core.core, instruction->data.integer.integer);

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
    bbAssert(0==1, "We should not get here\n");

    return bbSuccess;
}
bbFlag bbInstruction_uncheckActions_fn(bbCore* core, bbInstruction* instruction)
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
        //TODO place instruction->redo_instruction into core->action_queue
        bbAction* redo_action;
        bbVPool_lookup(core->action_pool, (void**)&redo_action, instruction->redo_instruction);
        bbList_sortL(&core->action_queue,(void*)redo_action);
        bbVPool_free(core->instruction_pool, (void*)instruction);
        return bbSuccess;
    }
    bbAssert(0==1, "We should not get here\n");
}



bbFlag bbInstruction_setTestInt_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unsetTestInt;
    undo_instruction->data.integer.integer = home.core.test_int;
    undo_instruction->source = instruction->source;


    printf("Set test_int: old = %lld, new = %lld\n", home.core.test_int,
        instruction->data.integer.integer);
    home.core.test_int = instruction->data.integer.integer;

    bbHandle no_handle = {0};
    if (home.core.test_int > 0)
    {
        bbCoreInput_setTestInt(core,
                          home.core.test_int-1,
                          bbInstructionSource_internal,
                          no_handle);
    }
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
    bbAssert(0==1, "We should not get here\n");
}
bbFlag bbInstruction_unsetTestInt_fn(bbCore* core, bbInstruction* instruction)
{
    printf("Unset test_int: old = %lld, new = %lld\n", home.core.test_int,
        instruction->data.integer.integer);
    home.core.test_int = instruction->data.integer.integer;

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
    bbAssert(0==1, "We should not get here\n");

}