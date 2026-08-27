

#include "engine/ECS/ECS.h"
#include "engine/ECS/bbEntitySpawner.h"
#include "engine/ECS/moveables/bbMoveables.h"
#include "engine/ECS/server_entities/bbServerEntities.h"
#include "engine/logic/bbFlag.h"
#include "moveables/moveables.h"

///Core synchronous spawn empty entity
bbFlag bbCS_spawnEmptyEntity(bbECS_entity** entity, bbInstruction_source source);
bbFlag bbSF_addServerEntity_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source);
bbFlag bbSF_addMoveable_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source);
bbFlag bbSF_addGraphics_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source);

bbFlag bbCS_spawnEmptyEntity(bbECS_entity** entity, bbInstruction_source source)
{
    bbAssert(source == bbInstructionSource_norewind, "not implemented");
    bbHere()
    bbCoreSynchronous_spawnEmptyEntity(&home.core.core,
                                       home.ECS.ECS,
                                       entity,
                                       "SKELLY",
                                       source,
                                       no_handle);



    return bbSuccess;
}


bbFlag bbSF_addServerEntity_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source)
{
    bbAssert(source == bbInstructionSource_norewind, "not implemented");
    bbHere()

    bbHandle handle;
    bbVPool_reverseLookup(home.ECS.ECS->system.pool, entity, &handle);

    bbCoreSynchronous_setServerEntity(&home.core.core,
                                   handle,
                                   args.server_handle,
                                  bbInstructionSource_norewind,
                                  no_handle);
    return bbSuccess;
}
bbFlag bbSF_addMoveable_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source)
{
    bbAssert(source == bbInstructionSource_norewind, "not implemented");

    bbHandle handle;
    bbVPool_reverseLookup(home.ECS.ECS->system.pool, entity, &handle);
    bbHandle moveable_handle;
    bbCoreSynchronous_spawnTestMoveable(&home.core.core,
                                           handle,
                                           &moveable_handle,
                                           args.position,
                                           args.speed,
                                           args.radius,
                                           args.mass,
                                           bbInstructionSource_norewind,
                                           no_handle);

    bbMoveable_setGoalPoint(&home.ECS.moveables,moveable_handle, args.goalpoint);

    bbHere()
    return bbSuccess;
}

bbFlag bbSF_addMoveable_skelly2(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source)
{
    bbAssert(source == bbInstructionSource_norewind, "not implemented");

    bbHandle handle;
    bbVPool_reverseLookup(home.ECS.ECS->system.pool, entity, &handle);
    bbHandle moveable_handle;
    bbCoreSynchronous_spawnTestMoveable(&home.core.core,
                                           handle,
                                           &moveable_handle,
                                           args.position,
                                           args.speed,
                                           args.radius,
                                           args.mass,
                                           bbInstructionSource_norewind,
                                           no_handle);
    bbMoveable_setGoalMoveable(&home.ECS.moveables,moveable_handle, args.goal_server_handle);


    bbHere()
    return bbSuccess;
}

bbFlag bbSF_addGraphics_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source)
{
    bbAssert(source == bbInstructionSource_norewind, "not implemented");
    bbHere()

    bbHandle handle;
    bbVPool_reverseLookup(home.ECS.ECS->system.pool, entity, &handle);

    bbHandle moveable = entity->components[bbECS_Moveables];
    //
    // bbCoreSynchronous_spawnGraphicsComponent(&home.core.core,
    //                                args.position,
    //                                handle,
    //                                moveable,
    //                                bbInstructionSource_norewind);

    bbCoreInput_spawnGraphicsComponent(&home.core.core,
                                   args.position,
                                   handle,
                                   moveable,
                                   bbInstructionSource_norewind,
                                   no_handle);

    return bbSuccess;
}

//scans a line from spawner.csv that looks like
//PARSER,UNUSED,POSITION I,J,K,GOALPOINT I, J, K, SERVER_HANDLE_INDEX,COLLISION
bbFlag bbPF_skellyParser(void* Spawner, char* string)
{
    bbEntitySpawner* spawner = (bbEntitySpawner*)Spawner;
    char key[KEY_LENGTH];
    char entity_type[KEY_LENGTH];
    bbSpawnFunctionArgs args;
    I32 num_chars;
    char spawn_functions[256];
    sscanf(string, "%[^','],%[^','],%d,%d,%d,%d,%d,%d,%d,%d,%n",
        key,entity_type,&args.position.i,&args.position.j,&args.position.k,
        &args.goalpoint.i,&args.goalpoint.j,&args.goalpoint.k,&args.server_handle.bloated.index,
        &args.server_handle.bloated.collision,&num_chars);

    bbECS_entity* entity;
    bbCS_spawnEmptyEntity(&entity, bbInstructionSource_norewind);

    char component[KEY_LENGTH];
    I32 component_length;
    while (1)
    {
        char separator;
        sscanf(&string[num_chars], "%[^',\n']%c%n",component,&separator,&component_length);

        num_chars += component_length;
        //printf("%s,",component);


        bbSpawnFunction* spawn_function;
        bbHandle handle;
        bbDictionary_lookup(spawner->spawn_dict, component, &handle);
        spawn_function = spawner->spawn_functions[handle.u64];

        spawn_function(spawner, entity, args,bbInstructionSource_norewind);
        if (separator == '\0' || separator == '\n') break;
    }

}

//scans a line from spawner.csv that looks like
//PARSER,UNUSED,POSITION I,J,K,GOALPOINT I, J, K, SERVER_HANDLE_INDEX,COLLISION,GOAL_SERVER_HANDLE_INDEX,COLLISION
bbFlag bbPF_skelly2Parser(void* Spawner, char* string)
{
    bbEntitySpawner* spawner = (bbEntitySpawner*)Spawner;
    char key[KEY_LENGTH];
    char entity_type[KEY_LENGTH];
    bbSpawnFunctionArgs args;
    I32 num_chars;
    char spawn_functions[256];
    sscanf(string, "%[^','],%[^','],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%n",
        key,entity_type,
        &args.position.i,&args.position.j,&args.position.k,
        &args.goalpoint.i,&args.goalpoint.j,&args.goalpoint.k,
        &args.server_handle.bloated.index,&args.server_handle.bloated.collision,
        &args.goal_server_handle.bloated.index,&args.goal_server_handle.bloated.collision,
        &num_chars);

    bbECS_entity* entity;
    bbCS_spawnEmptyEntity(&entity, bbInstructionSource_norewind);

    char component[KEY_LENGTH];
    I32 component_length;
    while (1)
    {
        char separator;
        sscanf(&string[num_chars], "%[^',\n']%c%n",component,&separator,&component_length);

        num_chars += component_length;
        //printf("%s,",component);


        bbSpawnFunction* spawn_function;
        bbHandle handle;
        bbDictionary_lookup(spawner->spawn_dict, component, &handle);
        spawn_function = spawner->spawn_functions[handle.u64];

        spawn_function(spawner, entity, args,bbInstructionSource_norewind);
        if (separator == '\0' || separator == '\n') break;
    }

}