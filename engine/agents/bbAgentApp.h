#ifndef BB_AGENTSAPP_H
#define BB_AGENTSAPP_H
#include "bbAvoidables.h"
#include "engine/agents/bbMoveables.h"

typedef struct
{
    bbMoveables movables;\
    bbAvoidables* avoidables;\
} bbAgentsApp;


#endif //BB_AGENTSAPP_H
