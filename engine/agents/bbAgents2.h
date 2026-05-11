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


#ifndef BB_AGENT2_H
#define BB_AGENT2_H

#include "engine/logic/bbHandle.h"
#include "engine/logic/bbList.h"
#include "engine/logic/bbFlag.h"
#include "engine/geometry/bbCoordinates.h"

#define NUM_AGENTS 8

typedef struct
{
    bbListElement_Handle full_list;
    bbListElement_Handle square_list;
    I32 moveable;
    bbMapCoords position;
    bbMapCoords goalpoint;
} bbAgent2;

typedef struct
{
    bbSquareCoords coords;
    bbList agents;
} bbAgents_square2;

typedef struct
{
    bbVPool* pool;
    bbList full_list;
    bbAgents_square2 lost_square;
    I32 squares_i;
    I32 squares_j;

    bbAgents_square2 squares[];
} bbAgents2;


///This instruction call screenshots all the transient data and then sets new values
bbFlag bbCoreInput_updateAgents();

bbFlag bbAgents2_new(bbAgents2** agents, I32 squares_i, I32 squares_j);



#endif //BB_AGENT2_H