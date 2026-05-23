#ifndef BB_AGENTSAPP_H
#define BB_AGENTSAPP_H
#include "engine/entities/bbAgentFunctions.h"
#include "engine/entities/bbAgents.h"
#include "engine/entities/bbAvoidables.h"
#include "engine/entities/bbMoveables.h"


typedef struct
{
    bbMoveables movables;
    bbAvoidables* avoidables;
    bbAgents* agents;
    I32 player_entity;
    bbAgentFunctions functions;
    bbEntities entities;
} bbAgentsApp;


#endif //BB_AGENTSAPP_H
