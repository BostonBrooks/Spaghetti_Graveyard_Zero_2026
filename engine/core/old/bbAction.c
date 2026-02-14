#include "engine/core/bbAction.h"

#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbTerminal.h"


bbFlag bbActions_init(bbActions* actions)
{
    bbVPool_newBloated(&actions->pool,sizeof(bbAction), 100,100);
    bbList_init(&actions->future_list,actions->pool,NULL,offsetof(bbAction,list_element),bbAction_compare);
}

///return A < B
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

bbFlag bbAction_setColor(bbActions* actions,
                         sfColor color,
                         U32 player,
                         U32 collision,
                         U64 created_tick,
                         U64 act_tick)
{
    bbAction* action;
    bbList_alloc(&actions->future_list,(void**)&action);

    action->player = player;
    action->collision = collision;
    action->type = bbActionType_setColor;
    action->created_tick = created_tick;
    action->act_tick = act_tick;

    bbList_sortL(&actions->future_list, action);
    return bbSuccess;

}



