

#include "engine/ECS/ECS.h"
#include "../../../../../engine/ECS/entity_spawner/bbEntitySpawner.h"
#include "engine/ECS/moveables/bbMoveables.h"
#include "engine/ECS/moveables/bbMoveables_setState.h"
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
    //bbHere()
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
    //bbHere()

    bbHandle handle;
    bbVPool_reverseLookup(home.ECS.ECS->system.pool, entity, &handle);

    bbCoreSynchronous_setServerEntity(&home.core.core,
                                   handle,
                                   args.handle,
                                  bbInstructionSource_norewind,
                                  no_handle);
    return bbSuccess;
}
bbFlag bbSF_addMoveable_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source)
{
    bbAssert(source == bbInstructionSource_norewind || source == bbInstructionSource_internal, "not implemented");

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
                                           source,
                                           no_handle);

    //We dont need to undo this, will be nuked by bbInstruction_unspawnTestMoveable_fn
    bbCI_Moveable_setIdle(&home.core.core,moveable_handle,source,no_handle);

    //bbHere()
    return bbSuccess;
}

bbFlag bbSF_addMoveable_skelly2(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source)
{
    bbAssert(source == bbInstructionSource_norewind || source == bbInstructionSource_internal, "not implemented");

    //bbDebug("speed = %d, radius = %d\n", args.speed, args.radius);

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
                                           source,
                                           no_handle);



    if (args.state == bbMoveableType_Following)
    {
        bbMoveable_setGoalMoveable(&home.ECS.moveables,moveable_handle, args.goal_handle);
    } else if (args.state == bbMoveableType_Idle)
    {
        bbCI_Moveable_setIdle(&home.core.core,moveable_handle,source,no_handle);
    } else
    {
        bbNotHere() //Not yet implemented
    }

    //We dont need to undo this, will be nuked by bbInstruction_unspawnTestMoveable_fn
    //bbHere()
    return bbSuccess;
}

bbFlag bbSF_addGraphics_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source)
{

    bbAssert(source == bbInstructionSource_norewind || source == bbInstructionSource_internal, "not implemented");

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
                                    "SKELLY",
                                   args.position,
                                   bbDrawableState_moving,
                                   handle,
                                   moveable,
                                   source,
                                   no_handle);

    return bbSuccess;
}

bbFlag bbSF_addGraphics_cow(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source)
{

    bbAssert(source == bbInstructionSource_norewind || source == bbInstructionSource_internal, "not implemented");

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
                                    "COW",
                                   args.position,
                                   bbDrawableState_moving,
                                   handle,
                                   moveable,
                                   source,
                                   no_handle);

    return bbSuccess;
}

bbFlag bbSF_addGraphics_lizard(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source)
{

    bbAssert(source == bbInstructionSource_norewind || source == bbInstructionSource_internal, "not implemented");

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
                                    "LIZARD",
                                   args.position,
                                   bbDrawableState_moving,
                                   handle,
                                   moveable,
                                   source,
                                   no_handle);

    return bbSuccess;
}

bbFlag bbSF_addGraphics_skelly2(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source)
{

    bbAssert(source == bbInstructionSource_norewind || source == bbInstructionSource_internal, "not implemented");

    bbHandle handle;
    bbVPool_reverseLookup(home.ECS.ECS->system.pool, entity, &handle);

    bbHandle moveable = entity->components[bbECS_Moveables];

    I32 state;
    if (args.state == bbMoveableType_Idle)
    {
        state = bbDrawableState_idle;
    } else
    {
        state = bbDrawableState_moving;
    }

    bbCoreInput_spawnGraphicsComponent(&home.core.core,
                                    "SKELLY",
                                   args.position,
                                   state,
                                   handle,
                                   moveable,
                                   source,
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
        &args.goalpoint.i,&args.goalpoint.j,&args.goalpoint.k,&args.handle.bloated.index,
        &args.handle.bloated.collision,&num_chars);

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
    char state[KEY_LENGTH];
    bbSpawnFunctionArgs args;
    I32 num_chars;
    char spawn_functions[256];
    sscanf(string, "%[^','],%[^','],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%n",
        key,state,
        &args.position.i,&args.position.j,&args.position.k,
        &args.goalpoint.i,&args.goalpoint.j,&args.goalpoint.k,
        &args.handle.bloated.index,&args.handle.bloated.collision,
        &args.goal_handle.bloated.index,&args.goal_handle.bloated.collision,&args.speed,&args.radius,&args.mass,
        &num_chars);

    ///POINTS_PER_PIXEL was 8
    args.position.i*=(POINTS_PER_PIXEL / 8);
    args.position.j*=(POINTS_PER_PIXEL / 8);
    args.position.k*=(POINTS_PER_PIXEL / 8);
    args.goalpoint.i*=(POINTS_PER_PIXEL / 8);
    args.goalpoint.j*=(POINTS_PER_PIXEL / 8);
    args.goalpoint.k*=(POINTS_PER_PIXEL / 8);


    bbHandle state_handle;
    bbDictionary_lookup(spawner->states, state, &state_handle);

    args.state = state_handle.u64;

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


bbFlag bbSF_addAI_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbSpawnFunctionArgs args,
                               bbInstruction_source source);

bbFlag bbLSF_liveSpawnSkelly(void* spawner,
                                  bbHandle* Entity,
                                  bbSpawnFunctionArgs args,
                                  bbInstruction_source source)
{
    bbAssert(source == bbInstructionSource_norewind || source == bbInstructionSource_internal, "not implemented");

    bbECS_entity* entity;

    bbCoreSynchronous_spawnEmptyEntity(&home.core.core,
                                   home.ECS.ECS,
                                   &entity,
                                   "SKELLY",
                                   source,
                                   no_handle);

    bbSF_addMoveable_skelly(spawner,
                               entity,
                               args,
                               source);


    bbSF_addGraphics_skelly2(spawner,
                               entity,
                               args,
                               source);

    bbSF_addAI_skelly(spawner,
                      entity,
                      args,
                      source);


    bbComponent_getHandle(&home.ECS.ECS->system,(bbComponent*)entity,Entity);
    return bbSuccess;
}