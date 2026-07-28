

#include "engine/ECS/ECS.h"
#include "engine/ECS/bbEntitySpawner.h"
#include "engine/ECS/bbServerEntities.h"
#include "engine/logic/bbFlag.h"

///Core synchronous spawn empty entity
bbFlag bbCS_spawnEmptyEntity(bbECS_entity** entity, bbInstruction_source source);
bbFlag bbSF_addServerEntity_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbMapCoords position,
                               bbMapCoords goalpoint,
                               bbHandle server_handle,
                               bbInstruction_source source);
bbFlag bbSF_addMoveable_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbMapCoords position,
                               bbMapCoords goalpoint,
                               bbHandle server_handle,
                               bbInstruction_source source);
bbFlag bbSF_addGraphics_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbMapCoords position,
                               bbMapCoords goalpoint,
                               bbHandle server_handle,
                               bbInstruction_source source);


bbFlag bbCS_spawnEmptyEntity(bbECS_entity** entity, bbInstruction_source source)
{
    bbAssert(source == bbInstructionSource_norewind, "not implemented");
    bbHere()
    bbCoreSynchronous_spawnEmptyEntity(&home.core.core,
                                       &home.ECS.ECS,
                                       entity,
                                       "SKELLY",
                                       bbInstructionSource_internal,
                                       no_handle);



    return bbSuccess;
}


bbFlag bbSF_addServerEntity_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbMapCoords position,
                               bbMapCoords goalpoint,
                               bbHandle server_handle,
                               bbInstruction_source source)
{
    bbAssert(source == bbInstructionSource_norewind, "not implemented");
    bbHere()

    bbHandle handle;
    bbVPool_reverseLookup(home.ECS.ECS.pool, entity, &handle);

    bbCoreSynchronous_setServerEntity(&home.core.core,
                                   handle,
                                   server_handle,
                                   bbInstructionSource_norewind);
    return bbSuccess;
}
bbFlag bbSF_addMoveable_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbMapCoords position,
                               bbMapCoords goalpoint,
                               bbHandle server_handle,
                               bbInstruction_source source)
{
    bbAssert(source == bbInstructionSource_norewind, "not implemented");

    bbHandle handle;
    bbVPool_reverseLookup(home.ECS.ECS.pool, entity, &handle);
    bbHandle moveable_handle;
    bbCoreSynchronous_spawnTestMoveable(&home.core.core,
                                           handle,
                                           &moveable_handle,
                                           position,
                                           bbInstructionSource_norewind,
                                           no_handle);

    bbHere()
    return bbSuccess;
}
bbFlag bbSF_addGraphics_skelly(void* spawner,
                               bbECS_entity* entity,
                               bbMapCoords position,
                               bbMapCoords goalpoint,
                               bbHandle server_handle,
                               bbInstruction_source source)
{
    bbAssert(source == bbInstructionSource_norewind, "not implemented");


    bbHandle handle;
    bbVPool_reverseLookup(home.ECS.ECS.pool, entity, &handle);

    bbHandle moveable = entity->components[bbECS_Moveables];

    bbCoreSynchronous_spawnGraphicsComponent(&home.core.core,
                                   position,
                                   handle,
                                   moveable,
                                   bbInstructionSource_norewind);
    bbHere()
    return bbSuccess;
}


bbFlag bbPF_skellyParser(void* Spawner, char* string)
{
    bbEntitySpawner* spawner = (bbEntitySpawner*)Spawner;
    char key[KEY_LENGTH];
    char entity_type[KEY_LENGTH];
    bbMapCoords position;
    bbMapCoords goalpoint;
    bbHandle server_handle;
    I32 num_chars;
    char spawn_functions[256];
    sscanf(string, "%[^','],%[^','],%d,%d,%d,%d,%d,%d,%d,%d,%n",
        key,entity_type,&position.i,&position.j,&position.k,
        &goalpoint.i,&goalpoint.j,&goalpoint.k,&server_handle.bloated.index,
        &server_handle.bloated.collision,&num_chars);

    bbECS_entity* entity;
    bbCS_spawnEmptyEntity(&entity, bbInstructionSource_norewind);

    char component[KEY_LENGTH];
    I32 component_length;
    while (1)
    {
        char separator;
        sscanf(&string[num_chars], "%[^',\n']%c%n",component,&separator,&component_length);
        if (separator == '\0' || separator == '\n') break;
        num_chars += component_length;
        //printf("%s,",component);


        bbSpawnFunction* spawn_function;
        bbHandle handle;
        bbDictionary_lookup(spawner->spawn_dict, component, &handle);
        spawn_function = spawner->spawn_functions[handle.u64];

        spawn_function(spawner, entity, position, goalpoint,server_handle,bbInstructionSource_norewind);
    }

}