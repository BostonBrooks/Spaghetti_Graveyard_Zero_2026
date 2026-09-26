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
                       U32 sender,
                       U32 collision,
                       bbTime created_tick,
                       bbTime act_tick);

#endif //ACTIONS_H