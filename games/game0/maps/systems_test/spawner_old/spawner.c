
#include <stdio.h>
#include "engine/data/bbHome.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/groundsurface/bbGroundSurface.h"
#include "engine/spawner/bbSpawner.h"

#include "games/game0/maps/systems_test/spawner/kitty.h"
#include "games/game0/maps/systems_test/spawner/balloon.h"
#include "games/game0/maps/systems_test/spawner/skelly.h"
#include  "games/game0/maps/systems_test/spawner/zombie.h"
#include "games/game0/maps/systems_test/spawner/fireball.h"

//typedef bbFlag bbSpawnFunction (char* string);
//(MC.i, MC.j, movable_index, entity_index);
bbFlag bbSF_null(I32 i_coord, I32 j_coord, I32 movable_index, I32 entity_index)
{
    bbDebug("NULL Spawn Function\n");
    return bbSuccess;
}


bbFlag bbPF_null(char* string)
{
    bbDebug("%s\n", string);
    return bbSuccess;
}

bbFlag bbPF_treeGraphics(char* string)
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
//
// bbFlag bbPF_treeCore(char* string)
// {
//     bbViewportApp* app = &home.viewport_app;
//     bbMapCoords MC;
//     char key[KEY_LENGTH];
//     sscanf(string, "%[^','],%d,%d", key, &MC.i, &MC.j);
//
//
//     MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
//
//
//
//     bbAvoidable_newCircle(home.agents_app.avoidables, MC, 193);
//     return bbSuccess;
// }

bbFlag bbPF_skeletonGraphics(char* string)
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
//
//
// bbFlag bbPF_skeletonCore(char* string)
// {
//
//     bbMapCoords MC;
//     I32 index;
//     char key[KEY_LENGTH];
//     sscanf(string, "%[^','],%d,%d,%d", key, &MC.i, &MC.j,&index);
//
//     MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
//
//
//     bbMovable* movable = &home.agents_app.movables.movables[index];
//
//     home.agents_app.movables.available = index+1;
//
//     movable->type = bbMovableType_Follow;
//     movable->position = MC;
//     movable->goalpoint = MC;
//
//     movable->speed = 4000;
//     movable->coords_a = bbMapCoords_getMilliCoords(movable->position);
//     movable->coords_b = bbMapCoords_getMilliCoords(movable->position);
//
//     movable->goal_movable = index%8;
//
//
//     bbDebug("SKELETON, movable = %d, entity = %d\n", index,0);
//
//     return bbSuccess;
// }
/*
bbFlag bbPF_zombieGraphics(char* string)
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
    unit->drawable.frames[2].handle.u64 = 614;

    bbHandle unit_handle;
    bbVPool_reverseLookup(home.viewport_app.units->pool,unit,&unit_handle);
    unit->enitity = home.agents_app.entities.num_entities_graphics++;
    home.agents_app.entities.entity[unit->enitity].unit = unit_handle;



    return bbSuccess;
}*/
/*
bbFlag bbPF_zombieGraphics(char* string)
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
// /*
// bbFlag bbPF_zombieCore(char* string)
// {
//     bbMapCoords MC;
//     I32 index;
//     char key[KEY_LENGTH];
//     sscanf(string, "%[^','],%d,%d,%d", key, &MC.i, &MC.j,&index);
//
//     MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
//
//
//
//     bbMovable* movable = &home.agents_app.movables.movables[index];
//
//     home.agents_app.movables.available = index+1;
//
//     movable->type = bbMovableType_Player;
//     movable->position = MC;
//     movable->goalpoint = MC;
//
//     movable->coords_a = bbMapCoords_getMilliCoords(movable->position);
//     movable->coords_b = bbMapCoords_getMilliCoords(movable->position);
//
//     movable->goal_movable = -1;
//
//
//
//     return bbSuccess;
// }
// */
// bbFlag bbPF_zombieCore(char* string)
// {
//
//     bbMapCoords MC;
//     I32 index;
//     char key[KEY_LENGTH];
//     sscanf(string, "%[^','],%d,%d,%d", key, &MC.i, &MC.j,&index);
//
//     MC.k = bbMapCoords_getElevation(&home.ground_surface, MC);
//
//     bbAgents* agents = home.agents_app.agents;
//     bbAgent* agent;
//
//     bbList_alloc(&agents->full_list, (void**)&agent);
//
//
//
//     agent->square_list.prev = agents->pool->null;
//     agent->square_list.next = agents->pool->null;
//     agent->movable = index;
//     agent->ftable.update = -1;
//     agent->ftable.command = bbAgentFunctions_getInt(&home.agents_app.functions,
//                              AgentCommand, "COMMAND_PLAYER");
//     agent->ftable.update = bbAgentFunctions_getInt(&home.agents_app.functions, AgentUpdate,"UPDATE_PLAYER");
//
//     bbMovable* movable = &home.agents_app.movables.movables[index];
//
//     agent->state = bbAgentState_Idle;
//     home.agents_app.movables.available = index+1;
//
//     movable->type = bbMovableType_Moving;
//     movable->position = MC;
//     movable->goalpoint = MC;
//
//     movable->coords_a = bbMapCoords_getMilliCoords(movable->position);
//     movable->coords_b = bbMapCoords_getMilliCoords(movable->position);
//
//     movable->goal_movable = index%8;
//
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
//     agent->entity = home.agents_app.entities.available++;
//     home.agents_app.entities.entity[agent->entity].agent = agent_handle;
//
//     home.agents_app.entities.entity[agent->entity].movable.u64 = index;
//
//
//     home.agents_app.player_entity = agent->entity;
//
//
//     bbDebug("ZOMBIE, movable = %d, entity = %d\n", index, agent->entity);
//
//     return bbSuccess;
// }
/*
bbFlag bbPF_skellyGraphics(char* string)
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
    unit->enitity = home.agents_app.entities.num_entities_graphics++;
    home.agents_app.entities.entity[unit->enitity].unit = unit_handle;

    return bbSuccess;
}*/
/*
bbFlag bbPF_skellyCore(char* string)
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
    agent->movable = index;
    agent->ftable.update = bbAgentFunctions_getInt(&home.agents_app.functions,
                             AgentUpdate, "UPDATE_SKELLY");
    agent->ftable.command = -1;
    bbMovable* movable = &home.agents_app.movables.movables[index];

    agent->state = bbAgentState_Idle;
    home.agents_app.movables.available = index+1;

    movable->type = bbMovableType_Idle;
    movable->position = MC;
    movable->goalpoint = MC;

    movable->coords_a = bbMapCoords_getMilliCoords(movable->position);
    movable->coords_b = bbMapCoords_getMilliCoords(movable->position);

    movable->goal_movable = index%8;


    bbSquareCoords square_coords = bbMapCoords_getSquareCoords(MC);
    agent->square_coords = square_coords;
    bbAgents_square* square = bbAgents_getSquare(agents, square_coords.i, square_coords.j);

    bbList_pushL(&square->agents,agent);
    bbList_pushL(&agents->full_list,agent);

    bbHandle agent_handle;
    bbVPool_reverseLookup(home.agents_app.agents->pool,agent,&agent_handle);
    agent->entity = home.agents_app.entities.num_entities_core++;
    home.agents_app.entities.entity[agent->entity].agent = agent_handle;

    home.agents_app.entities.entity[agent->entity].movable.u64 = index;


    bbDebug("SKELLY, movable = %d, entity = %d\n", index, agent->entity);

    return bbSuccess;
}

*/

bbFlag bbPF_entityGraphics(char* string)
{
    bbMapCoords MC;
    I32 movable_index;
    I32 entity_index;
    char key[KEY_LENGTH];
    char entity_type[KEY_LENGTH];
    sscanf(string, "%[^','],%[^','],%d,%d,%d,%d",
        key, entity_type, &MC.i, &MC.j,&movable_index, &entity_index);

    bbHandle spawn_fn_handle;
    bbFlag flag = bbDictionary_lookup(home.spawner.spawn_dict,entity_type,&spawn_fn_handle);
    bbAssert(flag == bbSuccess, "bbSpawnFunction not found\n");
    bbSpawnFunction* function = home.spawner.spawn_graphics[spawn_fn_handle.u64];

    return function(MC.i, MC.j, movable_index, entity_index);
}

bbFlag bbPF_entityCore(char* string)
{
    bbMapCoords MC;
    I32 movable_index;
    I32 entity_index;
    char key[KEY_LENGTH];
    char entity_type[KEY_LENGTH];
    sscanf(string, "%[^','],%[^','],%d,%d,%d,%d",
        key, entity_type, &MC.i, &MC.j,&movable_index, &entity_index);

    bbHandle spawn_fn_handle;
    bbFlag flag = bbDictionary_lookup(home.spawner.spawn_dict,entity_type,&spawn_fn_handle);
    bbAssert(flag == bbSuccess, "bbSpawnFunction not found\n");
    bbSpawnFunction* function = home.spawner.spawn_core[spawn_fn_handle.u64];

    return function(MC.i, MC.j, movable_index, entity_index);
    return bbSuccess;
}

bbFlag bbSpawner_populate(bbSpawner* spawner)
{
    bbParseFunction_add(spawner,bbPF_null, bbPF_null, "NULL");
    bbParseFunction_add(spawner,NULL, bbPF_treeGraphics, "TREE");
    bbParseFunction_add(spawner,NULL, bbPF_skeletonGraphics, "SKELETON");
    //bbParseFunction_add(spawner,bbPF_skellyCore, bbPF_skellyGraphics, "SKELLY");
    //bbParseFunction_add(spawner,bbPF_zombieCore, bbPF_zombieGraphics, "ZOMBIE");
    bbParseFunction_add(spawner,bbPF_entityCore, bbPF_entityGraphics, "ENTITY");

    bbSpawnFunction_add(spawner,(void*)bbSF_null,(void*)bbSF_null, "NULL");

    bbSpawnFunction_add(spawner,(void*)NULL,(void*)bbSF_kittyGraphics, "KITTY");
    //bbEntityFunction_add(spawner,bbEntity_newKitty, bbUIUnit_newKitty, "KITTY");


    bbSpawnFunction_add(spawner,(void*)NULL,(void*)bbSF_fireballGraphics, "FIREBALL");
    //bbEntityFunction_add(spawner,bbEntity_newFireball, bbUIUnit_newFireball, "FIREBALL");

    bbSpawnFunction_add(spawner,(void*)NULL,(void*)bbSF_balloonGraphics, "BALLOON");
    //bbEntityFunction_add(spawner,bbEntity_newBalloon, bbUIUnit_newBalloon, "BALLOON");



    bbSpawnFunction_add(spawner,(void*)NULL,(void*)bbSF_skellyGraphics, "SKELLY");
    //bbEntityFunction_add(spawner,bbEntity_newSkelly, bbUIUnit_newSkelly, "SKELLY");


    bbSpawnFunction_add(spawner,(void*)NULL,(void*)bbSF_zombieGraphics, "ZOMBIE");
    //bbEntityFunction_add(spawner,bbEntity_newZombie, bbUIUnit_newZombie, "ZOMBIE");
    return bbSuccess;
}
