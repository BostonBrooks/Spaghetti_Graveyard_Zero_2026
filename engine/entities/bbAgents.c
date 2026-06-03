#include "engine/entities/bbAgents.h"

#include "engine/entities/bbMovables.h"
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


/*
bbFlag bbAgent_newSkelly(bbAgents* agents, bbMapCoords position)
{
    bbAgent* self;

    bbList_alloc(&agents->full_list, (void**)&self);
    self->square_list.prev = agents->pool->null;
    self->square_list.next = agents->pool->null;

    bbHandle handle;
    bbVPool_reverseLookup(agents->pool,self,&handle);

    self->ftable.update = bbAgentFunctions_getInt(&home.agents_app.functions, AgentUpdate,"UPDATE_SKELLY");

    self->movable = bbMovables_newSkelly(&home.agents_app.movables, position, handle);

    bbMovable* movable = &home.agents_app.movables.movables[self->movable];

    bbUI_Inbox_NewSkelly(&home.UI.inbox, position, self->movable);


    movable->position = position;
    movable->goalpoint = position;

    movable->coords_a = bbMapCoords_getMilliCoords(movable->position);
    movable->coords_b = bbMapCoords_getMilliCoords(movable->position);
    movable->type = bbMovableType_Idle;

    bbSquareCoords square_coords = bbMapCoords_getSquareCoords(position);
    self->square_coords = square_coords;
    bbAgents_square* square = bbAgents_getSquare(agents, square_coords.i, square_coords.j);

    bbList_pushL(&square->agents,self);
    bbList_pushL(&agents->full_list,self);

    return bbSuccess;

}*/

bbFlag bbAgent_newBanana(bbAgents* agents,bbAgent** self, bbMapCoords position,
    I32 entity_index, I32 movable_index)
{

    bbMapCoords MC = position;


    bbDebug("spawn banana i = %d, j = %d, k = %d\n", MC.i, MC.j, MC.k);

    I32 index = movable_index;
    //char key[KEY_LENGTH];
    //sscanf(string, "%[^','],%d,%d,%d", key, &MC.i, &MC.j,&index);

    //MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);

    //bbAgents* agents = home.agents_app.agents;
    bbAgent* agent;

    bbList_alloc(&agents->full_list, (void**)&agent);



    agent->square_list.prev = agents->pool->null;
    agent->square_list.next = agents->pool->null;
    agent->movable = index;
    agent->ftable.update = bbAgentFunctions_getInt(&home.agents_app.functions, AgentUpdate,"UPDATE_SKELLY");
    agent->ftable.command = -1;


    bbHandle agent_handle;
    bbVPool_reverseLookup(home.agents_app.agents->pool,agent,&agent_handle);

    bbMovable* movable = &home.agents_app.movables.movables[index];

    agent->state = bbAgentState_Idle;
    home.agents_app.movables.available = index+1;

    movable->type = bbMovableType_Idle;
    movable->position = MC;
    movable->goalpoint = MC;
    movable->goalpoint.i +=5000;
    movable->coords_a = bbMapCoords_getMilliCoords(MC);
    movable->coords_b = bbMapCoords_getMilliCoords(MC);
    movable->goal_movable = 0;
    movable->agent2 = agent_handle;

    bbSquareCoords square_coords = bbMapCoords_getSquareCoords(MC);
    agent->square_coords = square_coords;
    bbAgents_square* square = bbAgents_getSquare(agents, square_coords.i, square_coords.j);

    bbList_pushL(&square->agents,agent);
    bbList_pushL(&agents->full_list,agent);

    agent->entity = entity_index;
    home.agents_app.entities.available = entity_index+1;
    home.agents_app.entities.entity[agent->entity].agent = agent_handle;

    home.agents_app.entities.entity[agent->entity].movable.u64 = index;



    bbUI_Inbox_NewBanana(&home.UI.inbox, MC, entity_index, movable_index);

    *self = agent;
    return bbSuccess;



}

bbFlag bbAgent_deleteBanana(bbAgents* agents,bbAgent* agent)
{
    bbDebug("deleteBanana\n")

    bbSquareCoords SC = agent->square_coords;
    bbAgents_square* square = bbAgents_getSquare(agents, SC.i, SC.j);

    bbList_remove(&agents->full_list,agent);
    bbList_remove(&square->agents,agent);

    bbEntity* entity = &home.agents_app.entities.entity[agent->entity];

    bbHandle null_agent;
    bbHandle null_movable;

    null_agent = home.agents_app.agents->pool->null;
    null_movable.u64 = UINT64_MAX;

    entity->agent = null_agent;
    entity->movable = null_movable;


    bbMovable* movable = &home.agents_app.movables.movables[agent->movable];
    movable->type = bbMovableType_Unused;




    bbVPool_free(agents->pool,agent);


    //delete banana
    //delete movable
    //delete entity
    //message UI_inbox to delete unit


}