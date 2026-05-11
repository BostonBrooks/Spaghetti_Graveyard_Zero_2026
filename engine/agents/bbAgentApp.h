#ifndef BB_AGENTSAPP_H
#define BB_AGENTSAPP_H
#include "engine/agents/bbAgents.h"
#include "engine/agents/bbAgents2.h"
#include "engine/agents/bbAvoidables.h"
#include "engine/agents/bbMoveables.h"
#include "engine/agents/bbAgents.h"

typedef struct
{
    bbMoveables movables;
    bbAvoidables* avoidables;
    bbAgents agents;
    bbAgents2* agents2;
} bbAgentsApp;


#endif //BB_AGENTSAPP_H
