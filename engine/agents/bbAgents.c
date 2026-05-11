#include "engine/agents/bbAgents.h"

#include "engine/data/bbConstants.h"
#include "engine/logic/bbBloatedPool.h"


bbAgents_square* bbAgents_getSquare(bbAgents* agents, I32 squares_i, I32 squares_j);

bbFlag bbAgents_new(bbAgents** agents, I32 squares_i, I32 squares_j)
{
    bbAgents* self;
    self = (bbAgents*)malloc(sizeof(bbAgents)+ squares_i*squares_j* sizeof (bbAgents_square));

    self->squares_i = squares_i;
    self->squares_j = squares_j;

    bbVPool_newBloated(&self->pool,sizeof(bbAgent),100,100,"AGENTS");
    bbList_init(&self->full_list,self->pool,NULL,offsetof(bbAgent, full_list),NULL);
    bbList_init(&self->lost_list,self->pool,NULL,offsetof(bbAgent, square_list),NULL);



    *agents = (bbAgents*)self;
}



bbAgents_square* bbAgents_getSquare(bbAgents* agents, I32 squares_i, I32 squares_j)
{
    I32 sqaure_index = squares_i*squares_j;
}