#include "engine/agents/bbAgents.h"

bbFlag bbAgents_init(bbAgents* agents)
{

    agents->current_agent = 0;
    for (I32 i = 0; i<NUM_AGENTS; i++)
    {
        agents->agents[i].goalpoint.i = 0;
        agents->agents[i].goalpoint.j = 0;
        agents->agents[i].goalpoint.k = 0;
    }
}