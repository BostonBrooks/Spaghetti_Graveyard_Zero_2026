
bbFlag bbUnit_newKitty(bbUnit** self, bbMapCoords MC, I32 movable_index, I32 entity_index)
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
                        "UNIT_ANIMATION_ANGLE",
                        &drawfunctionHandle);

    unit->drawable.frames[0].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[0].handle.u64 = 7;
    unit->drawable.frames[0].start_time=  -(rand()%60);
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

bbFlag bbSF_kittyGraphics(I32 i_coord, I32 j_coord, I32 movable_index, I32 entity_index)
{
    //bbDebug("i_coord = %d, j_coord = %d, movable_index = %d, entity_index = %d\n",
    //    i_coord, j_coord, movable_index, entity_index);
    bbMapCoords MC;
    MC.i = i_coord;
    MC.j = j_coord;
    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
    bbUnit* unit;
    bbUnit_newKitty(&unit, MC, movable_index, entity_index);
    return bbSuccess;
}



// bbFlag bbSF_kittyCore(I32 i_coord, I32 j_coord, I32 movable_index, I32 entity_index)
// {
//     bbMapCoords MC;
//     MC.i = i_coord;
//     MC.j = j_coord;
//     MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
//     bbAgent* agent;
//     bbAgent_newKitty(&agent, MC, movable_index, entity_index);
// }

///Spawn kitty during gameplay, doesn't care about syncing the core

// bbFlag bbEntity_newKitty(bbAgent** agent, I32 type_index, bbMapCoords MC,bbMapCoords goalpoint, I32 movable_index, I32 entity_index)
// {
//     bbAgent* agent1;
//     bbAgent_newKitty(&agent1, MC, movable_index, entity_index);
//     bbUI_Inbox_NewUnit(&home.UI.inbox, type_index, MC, entity_index, movable_index);
//     *agent = agent1;
//
//     bbAssert(agent1!=NULL, "bad spawn function\n");
//     return bbSuccess;
// }

bbFlag bbUIUnit_newKitty( bbMapCoords MC, I32 movable_index, I32 entity_index)
{
    bbUnit* unit;
    bbUnit_newKitty(&unit,MC, movable_index, entity_index);

    return bbSuccess;
}