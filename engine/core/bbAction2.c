#include "bbAction2.h"

#include "bbCoreInputs.h"
#include "engine/core/bbCore.h"
#include "engine/logic/bbString.h"
#include "engine/logic/bbTerminal.h"

I32 bbAction2_compare (void* A, void* B)
{
    bbAction2_header* a = (bbAction2_header*)A;
    bbAction2_header* b = (bbAction2_header*)B;

    if (a->act_tick < b->act_tick) return 1;
    if (a->act_tick > b->act_tick) return 0;
    if (a->collision < b->collision) return 1;
    if (a->collision > b->collision) return 0;
    if (a->player < b->player) return 1;
    if (a->player > b->player) return 0;

    bbNotHere()

    return -1;
}

bbFlag bbAction_printString(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            char* key)
{
    bbCore* core = (bbCore*)Core;
    bbAction2* action;
    bbList_alloc(&core->action_queue,(void**)&action);
    action->header.type = bbActionType_printString;
    action->header.player = player;
    action->header.collision = collision;
    action->header.created_tick = created_tick;
    action->header.act_tick = act_tick;
    bbStr_setStr(action->header.key, key, KEY_LENGTH);
    bbList_sortL(&core->action2_queue,(void*)action);

    return bbSuccess;
}

bbFlag bbAction2_update(void* Core)
{
    //find earliest pending instruction
    bbCore* core = (bbCore*)Core;
    bbAction2* action;
    bbList_peakL(&core->action2_queue, (void**)&action);

    U64 time_n = action->header.act_tick;
    //rewind until before action is meant to execute
    bbCore_rewindUntilTime(core, time_n);
    //fast-forward back to now
    bbCore_react(core);
    //bbInstruction_checkActions() and bbCore_react()
    bbCoreInput_checkActions2(core, core->actual_time, bbInstructionSource_input,no_handle);
    bbCore_react(core);
}