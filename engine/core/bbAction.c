#include "bbCoreInputs.h"
#include "engine/core/bbCore.h"
#include "engine/logic/bbString.h"
#include "engine/logic/bbTerminal.h"


bbFlag bbAction_printString(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            char* key)
{
    bbCore* core = (bbCore*)Core;
    bbAction* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->type = bbActionType_printString;
    action->player = player;
    action->collision = collision;
    action->created_tick = created_tick;
    action->act_tick = act_tick;
    bbStr_setStr(action->key, key, KEY_LENGTH);
    bbList_sortL(&core->action_queue,(void*)action);



    return bbSuccess;
}


bbFlag bbAction_setQuote(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            char* key)
{
    bbCore* core = (bbCore*)Core;
    bbAction* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->type = bbActionType_setQuote;
    action->player = player;
    action->collision = collision;
    action->created_tick = created_tick;
    action->act_tick = act_tick;
    bbStr_setStr(action->key, key, KEY_LENGTH);
    bbList_sortL(&core->action_queue,(void*)action);

    return bbSuccess;
}

bbFlag bbAction_unfreezeButton(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            char* key)
{
    bbCore* core = (bbCore*)Core;
    bbAction* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->type = bbActionType_unfreezeButton;
    action->player = player;
    action->collision = collision;
    action->created_tick = created_tick;
    action->act_tick = act_tick;
    bbStr_setStr(action->key, key, KEY_LENGTH);
    bbList_sortL(&core->action_queue,(void*)action);

    return bbSuccess;
}


bbFlag bbActions_reactOnce(void* Core, U64 tick_time)
{
    bbCore* core = (bbCore*)Core;
    bbAction* action;
    bbFlag flag = bbList_peakL(&core->action_queue,(void**)&action);
    if (flag == bbNone) return bbBreak;
    if (action->act_tick > tick_time) return bbBreak;
    if (action->act_tick < tick_time)
    {
        //TODO action->act_tick OR action->act_tick - 1?
        bbCore_rewindUntilTime(core,action->act_tick);
    }
    bbList_popL(&core->action_queue,(void**)&action);

    bbHandle handle;
    bbVPool_reverseLookup(core->action_pool,action,&handle);

    bbAction* test_action;
    bbVPool_lookup(core->action_pool, (void**)&test_action, handle);

    if (action->type == bbActionType_printString)
    {
        bbCoreInput_printString(core, action->key, false);
        bbCore_react(core);
        return bbSuccess;
    }
    if (action->type == bbActionType_setQuote)
    {
        bbCoreInput_setQuote(core,action->key,bbInstructionSource_action,handle);
        bbCore_react(core);
        return bbSuccess;
    }

    if (action->type == bbActionType_unfreezeButton)
    {
        bbCoreInput_unfreezeButton2(core, action->key, false);
        bbCore_react(core);
        return bbSuccess;
    }

    bbVPool_free(core->action_pool, action);
    return bbSuccess;


}


bbFlag bbActions_react(void* Core, U64 tick_time)
{
    bbCore* core = (bbCore*)Core;
    while (1)
    {
        bbFlag flag = bbActions_reactOnce(core, core->simulation_time);
        if (flag == bbBreak) return bbSuccess;
    }
}

I32 bbAction_compare (void* A, void* B)
{
    bbAction* a = (bbAction*)A;
    bbAction* b = (bbAction*)B;

    if (a->act_tick < b->act_tick) return 1;
    if (a->act_tick > b->act_tick) return 0;
    if (a->collision < b->collision) return 1;
    if (a->collision > b->collision) return 0;
    if (a->player < b->player) return 1;
    if (a->player > b->player) return 0;

    bbAssert(0==1, "bbAction_compare has a collision\n");

    return -1;
}