/**
    A Unit is something that moves around the map on the graphics thread.
    Movables calculate the movement of units.
    Agents control the behaviour of units at a higher level.

    Agents are either frozen or taking some action which is given by the state.
    Different types of agent will behave differently given the same state.

    Agents are part of the core and must be rewindable. We will use snapshots
    of the state of all agents, as we do with movables.

    Each agent should have a link to per-type data that stays constant and does
    not have to be screenshotted. (the link itself can be screenshotted)

    per type data includes things like indices for virtual functions and
    attribute data



*/


#ifndef BB_AGENT2_H
#define BB_AGENT2_H

#include "engine/logic/bbHandle.h"
#include "engine/logic/bbList.h"
#include "engine/logic/bbFlag.h"
#include "engine/geometry/bbCoordinates.h"


typedef enum
{
    bbAgentState_Idle,
    bbAgentState_Follow
} bbAgent_state;

typedef struct
{
    I32 update;
    I32 command;
} bbAgentFunctionTable;


typedef struct
{
    bbListElement_Handle full_list;
    bbListElement_Handle square_list;
    I32 movable;
    I32 entity;
    bbSquareCoords square_coords;
    bbAgent_state state;
    bbAgentFunctionTable ftable;
    I32 health;
    I32 max_health;
} bbAgent;

typedef struct
{
    bbSquareCoords coords;
    bbList agents;
} bbAgents_square;

typedef struct
{
    bbVPool* pool;
    bbList full_list;
    bbAgents_square lost_square;
    I32 squares_i;
    I32 squares_j;

    bbAgents_square squares[];
} bbAgents;



bbFlag bbAgents_new(bbAgents** agents, I32 squares_i, I32 squares_j);


//bbFlag bbAgent_newSkelly(bbAgents* agents, bbMapCoords position);
bbAgents_square* bbAgents_getSquare(bbAgents* agents, I32 i, I32 j);

bbFlag bbAgent_newBanana(bbAgents* agents,bbAgent** self, bbMapCoords position,
    I32 entity_index, I32 movable_index);

bbFlag bbAgent_deleteBanana(bbAgents* agents,bbAgent* agent);
#endif //BB_AGENT2_H