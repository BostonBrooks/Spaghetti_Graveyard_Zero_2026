/**
    A Unit is something that moves around the map on the graphics thread.
    Moveables calculate the movement of units.
    Agents control the behaviour of units at a higher level.

    Agents are either frozen or taking some action which is given by the state.
    Different types of agent will behave differently given the same state.

    Agents are part of the core and must be rewindable. We will use snapshots
    of the state of all agents, as we do with moveables.

    Each agent should have a link to per-type data that stays constant and does
    not have to be screenshotted. (the link itself can be screenshotted)

    per type data includes things like indices for virtual functions and
    attribute data



*/


#ifndef BB_AGENT_H
#define BB_AGENT_H
#include "engine/logic/bbFlag.h"
#include "engine/geometry/bbCoordinates.h"

#define NUM_AGENTS 8

typedef struct
{
    bbMapCoords goalpoint;
} bbAgent;



typedef struct
{
    bbAgent agents[NUM_AGENTS];

    I32 current_agent;
} bbAgents;


///This instruction call screenshots all the transient data and then sets new values
bbFlag bbCoreInput_updateAgents();

bbFlag bbAgents_init(bbAgents* agents);



#endif //BB_AGENT_H