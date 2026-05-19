
#include <stdio.h>

#include "engine/data/bbHome.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/groundsurface/bbGroundSurface.h"
#include "engine/spawner/bbSpawner.h"


//typedef bbFlag bbSpawnFunction (char* string);

bbFlag bbSF_null(char* string)
{
    bbDebug("%s\n", string);
    return bbSuccess;
}

bbFlag bbSF_treeGraphics(char* string)
{

    bbViewportApp* app = &home.viewport_app;
    bbMapCoords MC;
    char key[KEY_LENGTH];
    sscanf(string, "%[^','],%d,%d", key, &MC.i, &MC.j);


    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);


    bbDrawable* drawable;
    bbDrawable_newTree(&drawable, app->drawables, &home.UI.graphics,MC);
    bbMapIcon* mapicon;
    bbMapIcon_new(&mapicon, app->mapIcons,&home.UI.graphics, MC);

    return bbSuccess;
}

bbFlag bbSF_treeCore(char* string)
{
    bbViewportApp* app = &home.viewport_app;
    bbMapCoords MC;
    char key[KEY_LENGTH];
    sscanf(string, "%[^','],%d,%d", key, &MC.i, &MC.j);


    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);



    bbAvoidable_newCircle(home.agents_app.avoidables, MC, 193);
    return bbSuccess;
}

bbFlag bbSF_skeletonGraphics(char* string)
{

    bbViewportApp* app = &home.viewport_app;
    bbMapCoords MC;
    I32 index;
    char key[KEY_LENGTH];
    sscanf(string, "%[^','],%d,%d,%d", key, &MC.i, &MC.j,&index);


    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);

    bbUnit* unit;
    bbUnit_newSkeleton(&unit,home.viewport_app.units, &home.UI.graphics, MC, index);


    return bbSuccess;
}


bbFlag bbSF_skeletonCore(char* string)
{

    bbMapCoords MC;
    I32 index;
    char key[KEY_LENGTH];
    sscanf(string, "%[^','],%d,%d,%d", key, &MC.i, &MC.j,&index);

    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);


    bbMoveable* moveable = &home.agents_app.movables.moveables[index];

    home.agents_app.movables.available = index+1;

    moveable->type = bbMoveableType_Follow;
    moveable->position = MC;
    moveable->goalpoint = MC;

    moveable->coords_a = bbMapCoords_getMilliCoords(moveable->position);
    moveable->coords_b = bbMapCoords_getMilliCoords(moveable->position);

    moveable->goal_moveable = index%8;



    return bbSuccess;
}

bbFlag bbSF_zombieGraphics(char* string)
{

    bbViewportApp* app = &home.viewport_app;
    bbMapCoords MC;
    I32 index;
    char key[KEY_LENGTH];
    sscanf(string, "%[^','],%d,%d,%d", key, &MC.i, &MC.j,&index);


    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);

    bbUnit* unit;
    bbUnit_newSkelly(&unit,home.viewport_app.units, &home.UI.graphics, MC, index);

    unit->drawable.frames[0].handle.u64 = 6;

    bbHandle unit_handle;
    bbVPool_reverseLookup(home.viewport_app.units->pool,unit,&unit_handle);
    unit->enitity = home.entities.num_entities_graphics++;
    home.entities.entity[unit->enitity].unit = unit_handle;

    return bbSuccess;
}
/*
bbFlag bbSF_zombieGraphics(char* string)
{

    bbViewportApp* app = &home.viewport_app;
    bbMapCoords MC;
    I32 index;
    char key[KEY_LENGTH];
    sscanf(string, "%[^','],%d,%d,%d", key, &MC.i, &MC.j,&index);


    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);

    bbUnit* unit;
    bbUnit_newSkeleton(&unit,home.viewport_app.units, &home.UI.graphics, MC, index);
    unit->drawable.frames[0].handle.u64 = 10;



    bbHandle drawfunctionHandle;
    bbDictionary_lookup(home.UI.graphics.drawfunctions->dictionary,
                "MAPICON_TEST",
                &drawfunctionHandle);




    unit->drawable.frames[2].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[2].handle.u64 = 614;
    unit->drawable.frames[2].start_time =  -(rand()%6);
    unit->drawable.frames[2].framerate = 1;
    unit->drawable.frames[2].offset.x = 0;
    unit->drawable.frames[2].offset.y = 0;


    bbHandle unit_handle;
    bbVPool_reverseLookup(home.viewport_app.units->pool,unit,&unit_handle);
    unit->enitity = home.entities.num_entities_graphics++;
    home.entities.entity[unit->enitity].unit = unit_handle;
    return bbSuccess;
}*/
/*
bbFlag bbSF_zombieCore(char* string)
{
    bbMapCoords MC;
    I32 index;
    char key[KEY_LENGTH];
    sscanf(string, "%[^','],%d,%d,%d", key, &MC.i, &MC.j,&index);

    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);



    bbMoveable* moveable = &home.agents_app.movables.moveables[index];

    home.agents_app.movables.available = index+1;

    moveable->type = bbMoveableType_Player;
    moveable->position = MC;
    moveable->goalpoint = MC;

    moveable->coords_a = bbMapCoords_getMilliCoords(moveable->position);
    moveable->coords_b = bbMapCoords_getMilliCoords(moveable->position);

    moveable->goal_moveable = -1;



    return bbSuccess;
}
*/
bbFlag bbSF_zombieCore(char* string)
{

    bbMapCoords MC;
    I32 index;
    char key[KEY_LENGTH];
    sscanf(string, "%[^','],%d,%d,%d", key, &MC.i, &MC.j,&index);

    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);

    bbAgents* agents = home.agents_app.agents;
    bbAgent* agent;

    bbList_alloc(&agents->full_list, (void**)&agent);



    agent->square_list.prev = agents->pool->null;
    agent->square_list.next = agents->pool->null;
    agent->moveable = index;
    agent->ftable.update = -1;
    agent->ftable.command = bbAgentFunctions_getInt(&home.agents_app.functions,
                             AgentCommand, "COMMAND_PLAYER");
    agent->ftable.update = bbAgentFunctions_getInt(&home.agents_app.functions, AgentUpdate,"UPDATE_PLAYER");

    bbMoveable* moveable = &home.agents_app.movables.moveables[index];

    agent->state = bbAgentState_Idle;
    home.agents_app.movables.available = index+1;

    moveable->type = bbMoveableType_Player;
    moveable->position = MC;
    moveable->goalpoint = MC;

    moveable->coords_a = bbMapCoords_getMilliCoords(moveable->position);
    moveable->coords_b = bbMapCoords_getMilliCoords(moveable->position);

    moveable->goal_moveable = index%8;


    bbSquareCoords square_coords = bbMapCoords_getSquareCoords(MC);
    agent->square_coords = square_coords;
    bbAgents_square* square = bbAgents_getSquare(agents, square_coords.i, square_coords.j);

    bbList_pushL(&square->agents,agent);
    bbList_pushL(&agents->full_list,agent);

    bbHandle agent_handle;
    bbVPool_reverseLookup(home.agents_app.agents->pool,agent,&agent_handle);
    agent->entity = home.entities.num_entities_core++;
    home.entities.entity[agent->entity].agent = agent_handle;

    home.entities.entity[agent->entity].moveable.u64 = index;


    home.agents_app.player_entity = agent->entity;
    return bbSuccess;
}

bbFlag bbSF_skellyGraphics(char* string)
{

    bbViewportApp* app = &home.viewport_app;
    bbMapCoords MC;
    I32 index;
    char key[KEY_LENGTH];
    sscanf(string, "%[^','],%d,%d,%d", key, &MC.i, &MC.j,&index);


    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);

    bbUnit* unit;
    bbUnit_newSkelly(&unit,home.viewport_app.units, &home.UI.graphics, MC, index);

    bbHandle unit_handle;
    bbVPool_reverseLookup(home.viewport_app.units->pool,unit,&unit_handle);
    unit->enitity = home.entities.num_entities_graphics++;
    home.entities.entity[unit->enitity].unit = unit_handle;

    return bbSuccess;
}

bbFlag bbSF_skellyCore(char* string)
{

    bbMapCoords MC;
    I32 index;
    char key[KEY_LENGTH];
    sscanf(string, "%[^','],%d,%d,%d", key, &MC.i, &MC.j,&index);

    MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);

    bbAgents* agents = home.agents_app.agents;
    bbAgent* agent;

    bbList_alloc(&agents->full_list, (void**)&agent);
    agent->square_list.prev = agents->pool->null;
    agent->square_list.next = agents->pool->null;
    agent->moveable = index;
    agent->ftable.update = bbAgentFunctions_getInt(&home.agents_app.functions,
                             AgentUpdate, "UPDATE_SKELLY");
    agent->ftable.command = -1;
    bbMoveable* moveable = &home.agents_app.movables.moveables[index];

    agent->state = bbAgentState_Idle;
    home.agents_app.movables.available = index+1;

    moveable->type = bbMoveableType_Idle;
    moveable->position = MC;
    moveable->goalpoint = MC;

    moveable->coords_a = bbMapCoords_getMilliCoords(moveable->position);
    moveable->coords_b = bbMapCoords_getMilliCoords(moveable->position);

    moveable->goal_moveable = index%8;


    bbSquareCoords square_coords = bbMapCoords_getSquareCoords(MC);
    agent->square_coords = square_coords;
    bbAgents_square* square = bbAgents_getSquare(agents, square_coords.i, square_coords.j);

    bbList_pushL(&square->agents,agent);
    bbList_pushL(&agents->full_list,agent);

    bbHandle agent_handle;
    bbVPool_reverseLookup(home.agents_app.agents->pool,agent,&agent_handle);
    agent->entity = home.entities.num_entities_core++;
    home.entities.entity[agent->entity].agent = agent_handle;

    home.entities.entity[agent->entity].moveable.u64 = index;

    return bbSuccess;
}

bbFlag bbSpawner_populate(bbSpawner* spawner)
{
    bbSpawner_add(spawner,bbSF_null, bbSF_null, "NULL");
    bbSpawner_add(spawner,bbSF_treeCore, bbSF_treeGraphics, "TREE");
    bbSpawner_add(spawner,bbSF_skeletonCore, bbSF_skeletonGraphics, "SKELETON");
    bbSpawner_add(spawner,bbSF_skellyCore, bbSF_skellyGraphics, "SKELLY");
    bbSpawner_add(spawner,bbSF_zombieCore, bbSF_zombieGraphics, "ZOMBIE");
    return bbSuccess;
}
