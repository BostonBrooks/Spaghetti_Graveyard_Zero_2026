#include "../bbSpawner.h"

#include <stdlib.h>

#include "../logic/bbPrime.h"


bbFlag bbSpawner_init(bbSpawner* spawner, I32 num)
{
    spawner->spawn_core = calloc(num, sizeof(bbSpawnFunction*));
    spawner->spawn_graphics = calloc(num, sizeof(bbSpawnFunction*));
    spawner->spawn_functions_available = 0;
    bbDictionary_new(&spawner->dict, nextPrime(num));
    return bbSuccess;
}

bbFlag bbSpawner_add(bbSpawner* spawner,
    bbSpawnFunction* spawn_core, bbSpawnFunction* spawn_graphics, char* key )
{

    U32 available = spawner->spawn_functions_available++;
    spawner->spawn_core[available] = spawn_core;
    spawner->spawn_graphics[available] = spawn_graphics;
    bbHandle handle;
    handle.u64 = available;

    bbDictionary_add(spawner->dict, key, handle);

    return bbSuccess;
}


bbFlag bbSpawner_getCore(bbSpawner* spawner,
    bbSpawnFunction** function, char* key)
{
    bbHandle handle;
    bbDictionary_lookup(spawner->dict, key, &handle);
    *function = spawner->spawn_core[handle.u64];
    return bbSuccess;
}


bbFlag bbSpawner_getGraphics(bbSpawner* spawner,
    bbSpawnFunction** function, char* key)
{
    bbHandle handle;
    bbDictionary_lookup(spawner->dict, key, &handle);
    *function = spawner->spawn_graphics[handle.u64];
    return bbSuccess;
}