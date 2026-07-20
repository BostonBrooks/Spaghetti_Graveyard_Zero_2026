#include "engine/core/bbCore.h"
#include "engine/logic/bbFlag.h"
#include "engine/core/bbInstruction.h"

#include "bbAction.h"
#include "bbCoreInputs.h"

#ifdef DEFINE_SKELLYCHASE
#include "core/core_inputs.h"
#include "core/instructions.h"
#endif
#include "engine/data/bbHome.h"
#include "engine/logic/bbString.h"
#include "engine/network/bbNetworkApp.h"
#include "engine/userinterface/bbUI_Inbox.h"
extern char test_string[KEY_LENGTH];

U32 collision = 0;



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

}

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
    //bbDebug ("unset time was %lu, now is %lu, actual is %lu\n", core->simulation_time,
    //instruction->data.unsigned_long, core->actual_time);

    core->simulation_time = instruction->data.u64 ;




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

bbFlag bbInstruction_checkActions_fn(bbCore* core, bbInstruction* instruction)
{

    bbAction* action;
    bbFlag flag;

    //Go back and process late arrival actions

    flag = bbList_peakL(&core->action_queue,(void**)&action);

    if (flag != bbSuccess) // list empty
    {
        bbInstruction* undo_instruction;
        bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
        undo_instruction->type = bbInstruction_uncheckActions;
        //undo_instruction->data.unsigned_long = core->simulation_time;
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

        return bbSuccess;
    }
    if (action->header.act_tick < core->simulation_time) //or < the previous time this instruction was called?
    {
        bbCore_rewindUntil(core, action->header.act_tick-1);
        bbCore_react(core);
    }

    bbInstruction* undo_instruction;
    bbVPool_alloc(core->instruction_pool, (void**)&undo_instruction);
    undo_instruction->type = bbInstruction_uncheckActions;
    undo_instruction->data.u64 = core->simulation_time;
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


    //Reverse the order of objects in queue
    flag = bbList_popL(&core->action_queue,(void**)&action);
    while (flag == bbSuccess && action->header.act_tick <= core->simulation_time)
    {
        bbList_pushL(&core->action_temp_fifo,(void*)action);
        flag = bbList_popL(&core->action_queue,(void**)&action);
    }
    //(if we go too far along in the queue, undo last instruction
    if (flag == bbSuccess) bbList_pushL(&core->action_queue,(void*)action);

    //take from one lifo and add to another
    flag = bbList_popL(&core->action_temp_fifo,(void**)&action);
    while (flag == bbSuccess)
    {
        bbHandle handle;
        bbVPool_reverseLookup(core->action_pool,action,&handle);

        if (action->header.type == bbActionType_setString)
        {
            bbCoreInput_setString(core,action->header.key,bbInstructionSource_action,handle);

        }

        flag = bbList_popL(&core->action_temp_fifo,(void**)&action);
    }
    //may or may not need the following call
    //bbCore_react(core);
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
