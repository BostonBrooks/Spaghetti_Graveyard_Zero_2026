

#include <stdlib.h>
//
// bbFlag bbAgent_newBalloon(bbAgent** self, bbMapCoords MC,bbMapCoords goalpoint,  I32 movable_index, I32 entity_index)
// {
//
//
//     MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
//
//     bbAgents* agents = home.agents_app.agents;
//     bbAgent* agent;
//
//     bbList_alloc(&agents->full_list, (void**)&agent);
//     agent->square_list.prev = agents->pool->null;
//     agent->square_list.next = agents->pool->null;
//     agent->movable = movable_index;
//     agent->ftable.update = -1;
//     agent->ftable.command = -1;
//     bbMovable* movable = &home.agents_app.movables.movables[movable_index];
//
//     agent->state = bbAgentState_Idle;
//     home.agents_app.movables.available = movable_index+1;
//
//
//
//     movable->type = bbMovableType_Moving;
//     movable->position = MC;
//     movable->goalpoint = goalpoint;
//
//     movable->coords_a = bbMapCoords_getMilliCoords(movable->position);
//     movable->coords_b = bbMapCoords_getMilliCoords(movable->position);
//
//     movable->goal_movable = movable_index%8;
//     movable->speed = 3000;
//
//     bbSquareCoords square_coords = bbMapCoords_getSquareCoords(MC);
//     agent->square_coords = square_coords;
//     bbAgents_square* square = bbAgents_getSquare(agents, square_coords.i, square_coords.j);
//
//     bbList_pushL(&square->agents,agent);
//     bbList_pushL(&agents->full_list,agent);
//
//     bbHandle agent_handle;
//     bbVPool_reverseLookup(home.agents_app.agents->pool,agent,&agent_handle);
//     agent->entity = entity_index;
//         home.agents_app.entities.available = entity_index+1;
//     home.agents_app.entities.entity[agent->entity].agent = agent_handle;
//
//     home.agents_app.entities.entity[agent->entity].movable.u64 = movable_index;
//
//     *self = agent;
//
//     return bbSuccess;
// }
bbFlag bbUnit_newBalloon(bbUnit** self, bbMapCoords MC, I32 movable_index, I32 entity_index)
{

    bbVPool* pool = home.viewport_app.units->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbUnitSquare* unitSquare = bbDrawables_getSquare(home.viewport_app.units,SC.i, SC.j, home.viewport_app.units->squares_i, home.viewport_app.units->squares_j);

    bbUnit* unit;
    bbHandle unit_handle;
    bbFlag flag = bbVPool_alloc2(pool, (void**)&unit,&unit_handle);


    unit->drawable.coords = MC;
    bbHandle drawfunctionHandle;

    unit->prev_coords = MC;
    unit->prev_time = 0;
    unit->prev_goalpoint = MC;
    unit->next_coords = MC;
    unit->next_time = 2;
    unit->next_goalpoint = MC;


    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                        "UNIT_SPRITE",
                        &drawfunctionHandle);

    unit->drawable.frames[0].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[0].handle.u64 = 689 + rand()%11;
    unit->drawable.frames[0].start_time=  0;
    unit->drawable.frames[0].framerate = 1;
    unit->drawable.frames[0].offset.x = 0;
    unit->drawable.frames[0].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "DRAWABLE_SHADOW",
                    &drawfunctionHandle);

    unit->drawable.frames[1].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[1].handle.u64 = 612;
    unit->drawable.frames[1].start_time =  -(rand()%6);
    unit->drawable.frames[1].framerate = 1;
    unit->drawable.frames[1].offset.x = 0;
    unit->drawable.frames[1].offset.y = 0;

    for (I32 k = 2; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.frames[k].drawfunction = -1;
    }

    //home.agents_app.entities.movable_units[movable_index] = unit_handle;
    //home.agents_app.entities.entity[entity_index].unit = unit_handle;
    bbList_sortL(&unitSquare->list, unit);
    *self = unit;
    return bbSuccess;
}

bbFlag bbSF_balloonGraphics(I32 i_coord, I32 j_coord, I32 movable_index, I32 entity_index)
{
    //bbDebug("i_coord = %d, j_coord = %d, movable_index = %d, entity_index = %d\n",
    //    i_coord, j_coord, movable_index, entity_index);
    bbMapCoords MC;
    MC.i = i_coord;
    MC.j = j_coord;
    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
    bbUnit* unit;
    bbUnit_newBalloon(&unit, MC, movable_index, entity_index);
    return bbSuccess;
}


//
// bbFlag bbSF_balloonCore(I32 i_coord, I32 j_coord, I32 movable_index, I32 entity_index)
// {
//     bbMapCoords MC;
//     MC.i = i_coord;
//     MC.j = j_coord;
//     MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
//     bbAgent* agent;
//     bbAgent_newBalloon(&agent, MC,MC, movable_index, entity_index);
// }

///Spawn balloon during gameplay, doesn't care about syncing the core
//
// bbFlag bbEntity_newBalloon(bbAgent** agent, I32 type_index, bbMapCoords MC,bbMapCoords MC2, I32 movable_index, I32 entity_index)
// {
//     bbAgent* agent1;
//     bbAgent_newBalloon(&agent1, MC, MC2,movable_index, entity_index);
//
//     bbDebug("goal.i = %d, goal.j = %d\nposition.i = %d, position.j = %d\n", MC2.i, MC2.j,MC.i, MC.j);
//
//     bbUI_Inbox_NewUnit(&home.UI.inbox, type_index, MC, entity_index, movable_index);
//     bbAssert(agent1!=NULL, "bad spawn function\n");
//     bbDebug("agent = %p\n", agent1);
//     *agent = agent1;
//     return bbSuccess;
// }

bbFlag bbUIUnit_newBalloon( bbMapCoords MC, I32 movable_index, I32 entity_index)
{
    bbUnit* unit;
    bbUnit_newBalloon(&unit,MC, movable_index, entity_index);

    return bbSuccess;
}