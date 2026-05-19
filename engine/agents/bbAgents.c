#include "engine/agents/bbAgents.h"

#include "engine/agents/bbMoveables.h"
#include "engine/data/bbConstants.h"
#include "engine/data/bbHome.h"
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
    bbList_init(&self->lost_square.agents,self->pool,NULL,offsetof(bbAgent, square_list),NULL);

    for (I32 i=0;i<squares_i;i++)
    {
        for (I32 j=0;j<squares_j;j++)
        {
            bbAgents_square* square;
            square = bbAgents_getSquare(self, i, j);

            bbList_init(&square->agents,self->pool,NULL,offsetof(bbAgent, square_list),NULL);
            square->coords.i = i;
            square->coords.j = j;
        }
    }

    *agents = self;
}



bbAgents_square* bbAgents_getSquare(bbAgents* agents, I32 i, I32 j)
{

    if (i<0 || i>=agents->squares_i) return &agents->lost_square;
    if (j<0 || j>=agents->squares_j) return &agents->lost_square;

    I32 index = i + agents->squares_i * j;

    return &agents->squares[index];
}



bbFlag bbAgent_newSkelly(bbAgents* agents, bbMapCoords position)
{
    bbAgent* self;

    bbList_alloc(&agents->full_list, (void**)&self);
    self->square_list.prev = agents->pool->null;
    self->square_list.next = agents->pool->null;

    bbHandle handle;
    bbVPool_reverseLookup(agents->pool,self,&handle);

    self->ftable.update = bbAgentFunctions_getInt(&home.agents_app.functions, AgentUpdate,"UPDATE_SKELLY");

    self->moveable = bbMoveables_newSkelly(&home.agents_app.movables, position, handle);

    bbMoveable* moveable = &home.agents_app.movables.moveables[self->moveable];

    bbUI_Inbox_NewSkelly(&home.UI.inbox, position, self->moveable);


    moveable->position = position;
    moveable->goalpoint = position;

    moveable->coords_a = bbMapCoords_getMilliCoords(moveable->position);
    moveable->coords_b = bbMapCoords_getMilliCoords(moveable->position);
    moveable->type = bbMoveableType_Idle;

    bbSquareCoords square_coords = bbMapCoords_getSquareCoords(position);
    self->square_coords = square_coords;
    bbAgents_square* square = bbAgents_getSquare(agents, square_coords.i, square_coords.j);

    bbList_pushL(&square->agents,self);
    bbList_pushL(&agents->full_list,self);

    return bbSuccess;

}

