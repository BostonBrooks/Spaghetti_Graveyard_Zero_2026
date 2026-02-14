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
    bbList_popL(&core->action_queue,(void**)&action);

    bbDebug("action->key = %s\n", action->key);

    bbVPool_free(core->action_pool, action);
    return bbSuccess;


}


bbFlag bbActions_react(void* Core, U64 tick_time)
{
    bbDebug("Tick time = %llu\n", tick_time);
    while (1)
    {
        bbFlag flag = bbActions_reactOnce(Core, tick_time);
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