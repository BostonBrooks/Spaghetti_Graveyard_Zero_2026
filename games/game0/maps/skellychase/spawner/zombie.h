
//
// bbFlag bbAgent_newZombiex(bbAgent** self, bbMapCoords MC,bbMapCoords goalpoint, I32 movable_index, I32 entity_index)
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
//     agent->ftable.update = bbAgentFunctions_getInt(&home.agents_app.functions,
//                              AgentUpdate, "UPDATE_PLAYER");;
//     agent->ftable.command = bbAgentFunctions_getInt(&home.agents_app.functions,
//                              AgentCommand, "COMMAND_PLAYER");
//     bbMovable* movable = &home.agents_app.movables.movables[movable_index];
//
//     agent->state = bbAgentState_Idle;
//     home.agents_app.movables.available = movable_index+1;
//
//     movable->type = bbMovableType_Moving;
//     movable->position = MC;
//     movable->goalpoint = MC;
//
//     movable->coords_a = bbMapCoords_getMilliCoords(movable->position);
//     movable->coords_b = bbMapCoords_getMilliCoords(movable->position);
//
//     movable->goal_movable = movable_index%8;
//     movable->speed = 6000;
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
bbFlag bbUnit_newZombiex(bbUnit** self, bbMapCoords MC, I32 movable_index, I32 entity_index)
{

    bbVPool* pool = home.viewport_app.units->pool;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbUnitSquare* unitSquare = bbDrawables_getSquare(home.viewport_app.units,SC.i, SC.j, home.viewport_app.units->squares_i, home.viewport_app.units->squares_j);

    bbUnit* unit;
    bbFlag flag = bbVPool_alloc(pool, (void**)&unit);

    bbHandle unit_handle;
    bbVPool_reverseLookup(pool, unit, &unit_handle);

    unit->drawable.coords = MC;
    unit->drawable.state = bbDrawableState_idle;
    bbHandle drawfunctionHandle;

    unit->prev_coords = MC;
    unit->prev_time = 0;
    unit->prev_goalpoint = MC;
    unit->next_coords = MC;
    unit->next_time = 2;
    unit->next_goalpoint = MC;


    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                        "COMPOSITION_STATE",
                        &drawfunctionHandle);

    unit->drawable.frames[0].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[0].handle.u64 = 6;
    unit->drawable.frames[0].start_time= 0;
    unit->drawable.frames[0].framerate = 1;
    unit->drawable.frames[0].offset.x = 0;
    unit->drawable.frames[0].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                    "DRAWABLE_SHADOW",
                    &drawfunctionHandle);

    unit->drawable.frames[1].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[1].handle.u64 = 612;
    unit->drawable.frames[1].start_time =  0;
    unit->drawable.frames[1].framerate = 1;
    unit->drawable.frames[1].offset.x = 0;
    unit->drawable.frames[1].offset.y = 0;

    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
             "MAPICON_TEST",
             &drawfunctionHandle);




    unit->drawable.frames[2].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[2].handle.u64 = 614;
    unit->drawable.frames[2].start_time =  0;
    unit->drawable.frames[2].framerate = 1;
    unit->drawable.frames[2].offset.x = 0;
    unit->drawable.frames[2].offset.y = 0;

    for (I32 k = 3; k < FRAMES_PER_DRAWABLE; k++){
        unit->drawable.frames[k].drawfunction = -1;
    }

    //home.agents_app.entities.movable_units[movable_index] = unit_handle;
    //home.agents_app.entities.entity[entity_index].unit = unit_handle;
    bbList_sortL(&unitSquare->list, unit);
    *self = unit;
    return bbSuccess;
}

bbFlag bbSF_zombieGraphics(I32 i_coord, I32 j_coord, I32 movable_index, I32 entity_index)
{
    //bbDebug("i_coord = %d, j_coord = %d, movable_index = %d, entity_index = %d\n",
    //    i_coord, j_coord, movable_index, entity_index);
    bbMapCoords MC;
    MC.i = i_coord;
    MC.j = j_coord;
    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
    bbUnit* unit;
    bbUnit_newZombiex(&unit, MC, movable_index, entity_index);
    return bbSuccess;
}



// bbFlag bbSF_zombieCore(I32 i_coord, I32 j_coord, I32 movable_index, I32 entity_index)
// {
//     bbMapCoords MC;
//     MC.i = i_coord;
//     MC.j = j_coord;
//     MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
//     bbAgent* agent;
//     bbAgent_newZombiex(&agent, MC,MC, movable_index, entity_index);
// }

///Spawn zombie during gameplay, doesn't care about syncing the core
//
// bbFlag bbEntity_newZombie(bbAgent** agent, I32 type_index, bbMapCoords MC,bbMapCoords MC2, I32 movable_index, I32 entity_index)
// {
//     bbAgent* agent1;
//     bbAgent_newZombiex(&agent1, MC,MC, movable_index, entity_index);
//     bbUI_Inbox_NewUnit(&home.UI.inbox, type_index, MC, entity_index, movable_index);
//     *agent = agent1;
//
//     bbAssert(agent1!=NULL, "bad spawn function\n");
//     return bbSuccess;
// }

bbFlag bbUIUnit_newZombie( bbMapCoords MC, I32 movable_index, I32 entity_index)
{
    bbUnit* unit;
    bbUnit_newZombiex(&unit,MC, movable_index, entity_index);

    return bbSuccess;
}