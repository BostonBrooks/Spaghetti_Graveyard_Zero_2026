#include "engine/viewport/bbViewportSpawner.h"

bbFlag bbViewportSpawner_init(bbViewportSpawner* viewport_spawner)
{

    viewport_spawner->spawn_functions = calloc(viewport_spawn_function_count,sizeof(bbViewportSpawnFunction*));
    viewport_spawner->spawn_function_count = 0;
    bbDictionary_new(&viewport_spawner->spawn_function_dict, viewport_spawn_function_count);
}

bbFlag bbViewportSpawner_add(bbViewportSpawner* spawner,
                             bbViewportSpawnFunction* spawn_function,
                             char* key )
{
    U32 available = spawner->spawn_function_count++;
    spawner->spawn_functions[available] = spawn_function;
    bbHandle handle;
    handle.u64 = available;
    bbDictionary_add(spawner->spawn_function_dict, key, handle);
    return bbSuccess;
}

I32 bbViewportSpawner_lookupInt(bbViewportSpawner* spawner,
                                char* key )
{
    bbHandle handle;
    bbDictionary_lookup(spawner->spawn_function_dict,
                        key,
                        &handle);

    return handle.u64;
}

//bbFlag bbViewportSpawner_populate(bbViewportSpawner* viewport_spawner);
bbFlag bbViewportSpawner_spawnDrawable(bbViewportApp* viewport,
                                     bbViewportSpawner* spawner,
                                     U32 type,
                                     bbMapCoords MC,
                                     bbHandle entity_handle,
                                     bbHandle moveable_handle)
{
    bbViewportSpawnFunction* function = spawner->spawn_functions[type];
    function(viewport,
             MC,
             entity_handle,
             moveable_handle);

    return bbSuccess;
}