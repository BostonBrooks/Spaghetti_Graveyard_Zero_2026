#include "engine/agents/bbAgents2.h"

#include "engine/agents/bbAgents.h"
#include "engine/agents/bbMoveables.h"
#include "engine/data/bbConstants.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbBloatedPool.h"


bbAgents_square2* bbAgents2_getSquare(bbAgents2* agents, I32 squares_i, I32 squares_j);

bbFlag bbAgents2_new(bbAgents2** agents, I32 squares_i, I32 squares_j)
{
    bbAgents2* self;
    self = (bbAgents2*)malloc(sizeof(bbAgents2)+ squares_i*squares_j* sizeof (bbAgents_square2));

    self->squares_i = squares_i;
    self->squares_j = squares_j;

    bbVPool_newBloated(&self->pool,sizeof(bbAgent2),100,100,"AGENTS");
    bbList_init(&self->full_list,self->pool,NULL,offsetof(bbAgent2, full_list),NULL);
    bbList_init(&self->lost_square.agents,self->pool,NULL,offsetof(bbAgent2, square_list),NULL);

    for (I32 i=0;i<squares_i;i++)
    {
        for (I32 j=0;j<squares_j;j++)
        {
            bbAgents_square2* square;
            square = bbAgents2_getSquare(self, i, j);

            bbList_init(&square->agents,self->pool,NULL,offsetof(bbAgent2, square_list),NULL);
            square->coords.i = i;
            square->coords.j = j;
        }
    }

    *agents = self;
}



bbAgents_square2* bbAgents2_getSquare(bbAgents2* agents, I32 i, I32 j)
{

    if (i<0 || i>=agents->squares_i) return &agents->lost_square;
    if (j<0 || j>=agents->squares_j) return &agents->lost_square;

    I32 index = i + agents->squares_i * j;

    return &agents->squares[index];
}



bbFlag bbAgent2_newTux(bbAgents2* agents, bbMapCoords position)
{
    bbAgent2* self;

    bbList_alloc(&agents->full_list, (void**)&self);
    self->square_list.prev = agents->pool->null;
    self->square_list.next = agents->pool->null;

    bbHandle handle;
    bbVPool_reverseLookup(agents->pool,self,&handle);


    self->moveable = bbMoveables_newTux(&home.agents_app.movables, position, handle);

    bbMoveable* moveable = &home.agents_app.movables.moveables[self->moveable];

    bbUI_Inbox_NewTux(&home.UI.inbox, position, self->moveable);


    moveable->position = position;
    moveable->goalpoint = position;

    moveable->coords_a = bbMapCoords_getMilliCoords(moveable->position);
    moveable->coords_b = bbMapCoords_getMilliCoords(moveable->position);
    moveable->type = bbMoveableType_Idle;

    bbSquareCoords square_coords = bbMapCoords_getSquareCoords(position);
    self->square_coords = square_coords;
    bbAgents_square2* square = bbAgents2_getSquare(agents, square_coords.i, square_coords.j);

    bbList_pushL(&square->agents,self);
    bbList_pushL(&agents->full_list,self);

    return bbSuccess;

}

