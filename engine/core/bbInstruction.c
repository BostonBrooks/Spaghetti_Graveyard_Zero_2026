#include "engine/core/bbCore.h"
#include "engine/logic/bbFlag.h"
#include "engine/core/bbInstruction.h"

#include "bbAction.h"
#include "bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbString.h"
#include "engine/network/bbNetworkApp.h"
#include "engine/userinterface/bbUI_Inbox.h"
extern char test_string[KEY_LENGTH];

bbFlag bbInstruction_netsendButton_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_netsendButton(&home.network, instruction->data.string);
    return bbSuccess;
}

bbFlag bbInstruction_netpauseButton_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_netpauseButton(&home.network, instruction->data.string);
    return bbSuccess;
}

bbFlag bbInstruction_netcodeButton_fn(bbCore* core, bbInstruction* instruction)
{
    bbNetworkApp_netcodeButton(&home.network, instruction->data.string, instruction->act_time);
    return bbSuccess;
}

bbFlag bbInstruction_loopAction_fn(bbCore* core, bbInstruction* instruction)
{
    bbDebug("Loop instruction at time = %lu, simulation time = %lu\n", instruction->act_time, core->simulation_time);
    bbAction_loop(core,
                  0,
                  rand(),
                  0,
                  instruction->act_time + 1,
                  instruction->data.string);
    return bbSuccess;

}

bbFlag bbInstruction_setString_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unsetString;
    bbStr_setStr(undo_instruction->data.string, test_string, KEY_LENGTH);
    undo_instruction->source = instruction->source;

    printf("    +old quote: %s, new quote %s, time = %lu\n",test_string,
        instruction->data.string, core->simulation_time);

    bbStr_setStr(test_string, instruction->data.string, KEY_LENGTH);


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
        instruction->data.string,test_string, core->simulation_time);

    bbStr_setStr(test_string,instruction->data.string, KEY_LENGTH);

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

bbFlag bbInstruction_unfreezeButton_fn(bbCore* core, bbInstruction* instruction)
{

bbDebug("unfreeze button %s\n", instruction->data.string);

    bbUI_Inbox_UnpressButton2(&home.UI.inbox, instruction->data.string);

    return bbSuccess;
    /* No need to undo this action?
    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    //undo_instruction->type = bbInstruction_unsetString;
    //bbStr_setStr(undo_instruction->data.string, test_string, KEY_LENGTH);
    undo_instruction->source = instruction->source;

    // printf("    +old quote: %s, new quote %s, time = %lu\n",test_string,
    //    instruction->data.string, core->simulation_time);

    //bbStr_setStr(test_string, instruction->data.string, KEY_LENGTH);


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
*/
}

bbFlag bbInstruction_setTime_fn(bbCore* core, bbInstruction* instruction)
{

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_unsetTime;
    undo_instruction->data.unsigned_long = core->simulation_time;
    undo_instruction->source = instruction->source;


    core->simulation_time = instruction->data.unsigned_long ;

    //printf("+time = %lu\n", core->simulation_time);
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
    core->simulation_time = instruction->data.unsigned_long ;

    //printf("-time = %lu\n", core->simulation_time);
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

///check actions using the new algorithm
bbFlag bbInstruction_checkActions_fn(bbCore* core, bbInstruction* instruction)
{





    bbAction* action;

    bbFlag flag = bbList_peakL(&core->action_queue,(void**)&action);

    if (flag != bbSuccess) return bbBreak;
    if (action->header.act_tick > core->simulation_time) return bbBreak;

    if (action->header.act_tick < core->simulation_time)
    {
        bbCore_rewindUntil(core, action->header.act_tick-1);
        bbCore_react(core);
    }

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_uncheckActions;
    undo_instruction->data.unsigned_long = core->simulation_time;
    undo_instruction->source = instruction->source;

    if (instruction->source == bbInstructionSource_internal)
    {
        bbVPool_free(core->instruction_pool, (void*)instruction);
        undo_instruction->redo_instruction.u64 = 0;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);

    }
    if (instruction->source == bbInstructionSource_input)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->instruction_pool, instruction, &handle);
        undo_instruction->redo_instruction = handle;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);

    }
    if (instruction->source == bbInstructionSource_action)
    {
        undo_instruction->redo_instruction = instruction->redo_instruction;
        bbList_pushL(&core->undo_stack,(void*)undo_instruction);

    }

   flag = bbList_popL(&core->action_queue,(void**)&action);

    if (flag != bbSuccess) return bbBreak;

    bbHandle handle;
    bbVPool_reverseLookup(core->action_pool,action,&handle);

    //Loop by placing self on stack
    bbCoreInput_checkActions(core, core->simulation_time,bbInstructionSource_internal,no_handle);

    bbWarning(action->header.act_tick <= core->simulation_time, "action  executed early\n"
        "action->header.act_tick = %lu core->simulation_time = %lu\n",
        action->header.act_tick, core->simulation_time);

    if (action->header.type == bbActionType_setString)
    {
        bbCoreInput_setString(core,action->header.key,bbInstructionSource_action,handle);
        bbCore_react(core);

    }

    if (action->header.type == bbActionType_unfreezeButton)
    {
        bbDebug("unfreeze button %s\n", action->header.key);
        bbCoreInput_unfreezeButton(core, action->header.key, bbInstructionSource_action,handle);
        bbCore_react(core);

    }

    if (action->header.type == bbActionType_loop)
    {
        bbCoreInput_loop(core,action->header.key,action->header.act_tick,bbInstructionSource_action,handle);
        bbCore_react(core);

    }

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




    return bbSuccess;
}