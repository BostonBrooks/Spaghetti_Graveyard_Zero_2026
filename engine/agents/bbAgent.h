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





*/


#ifndef BB_AGENT_H
#define BB_AGENT_H



#endif //BB_AGENT_H