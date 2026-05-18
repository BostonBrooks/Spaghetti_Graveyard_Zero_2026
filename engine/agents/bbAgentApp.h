#ifndef BB_AGENTSAPP_H
#define BB_AGENTSAPP_H
#include "engine/agents/bbAgentFunctions.h"
#include "engine/agents/bbAgents.h"
#include "engine/agents/bbAvoidables.h"
#include "engine/agents/bbMoveables.h"

typedef struct
{
    bbMoveables movables;
    bbAvoidables* avoidables;
    bbAgents* agents2;
    bbAgent* player;
    bbAgentFunctions functions;
} bbAgentsApp;


#endif //BB_AGENTSAPP_H
