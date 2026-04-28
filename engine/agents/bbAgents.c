#include "engine/agents/bbAgents.h"

#include "engine/data/bbConstants.h"

bbFlag bbAgents_init(bbAgents* agents)
{

    agents->current_agent = 0;
    for (I32 i = 0; i<NUM_AGENTS; i++)
    {
        agents->agents[i].goalpoint.i = (i%8)*POINTS_PER_TILE*4+10208;
        agents->agents[i].goalpoint.j = (i/8)*POINTS_PER_TILE*4+10208;
        agents->agents[i].goalpoint.k = 0;
    }
}
