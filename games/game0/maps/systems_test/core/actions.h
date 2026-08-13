#ifndef ACTIONS_H
#define ACTIONS_H
#include "engine/core/bbAction.h"

typedef enum
{

    bbActionType_setGoalpoint = bbActionType_numActions,
    bbActionType_bbHere,
    bbActionType_numVActions
} bbAction_vtype;

bbFlag bbAction_bbHere(void* Core,
                       U32 player,
                       U32 collision,
                       U64 created_tick,
                       U64 act_tick);

#endif //ACTIONS_H