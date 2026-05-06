

#include <stdio.h>
#include <stdlib.h>

#include "engine/spawner/bbSpawner.h"

#include "engine/logic/bbPrime.h"
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbTerminal.h"


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

bbFlag bbSpawner_spawnCore(bbSpawner* spawner, char* file_name)
{
    FILE* file = fopen(file_name, "r");
    bbAssert(file != NULL, "bad spawner file\n");

    char file_line[1024];

    char key[KEY_LENGTH];

    while (1)
    {
        char* flag = fgets(file_line, 1024, file);
        if (flag == NULL) break;
        I32 i = 0;
        while(1)
        {
            if (file_line[i] == ','|| file_line[i] == '\n' || file_line[i] == '\0'|| i == KEY_LENGTH-1)
            {
                key[i] = '\0';
                break;
            }
            key[i] = file_line[i];
            i++;
        }

        bbSpawnFunction* spawn_function;
        bbHandle handle;
        bbDictionary_lookup(spawner->dict, key, &handle);
        spawn_function = spawner->spawn_core[handle.u64];

        spawn_function(file_line);
    }

    return bbSuccess;
}

bbFlag bbSpawner_spawnGraphics(bbSpawner* spawner, char* file_name)
{
    FILE* file = fopen(file_name, "r");
    bbAssert(file != NULL, "bad spawner file\n");

    char file_line[1024];

    char key[KEY_LENGTH];

    while (1)
    {
        char* flag = fgets(file_line, 1024, file);
        if (flag == NULL) break;
        I32 i = 0;
        while(1)
        {
            if (file_line[i] == ','|| file_line[i] == '\n' || file_line[i] == '\0'|| i == KEY_LENGTH-1)
            {
                key[i] = '\0';
                break;
            }
            key[i] = file_line[i];
            i++;
        }

        bbSpawnFunction* spawn_function;
        bbHandle handle;
        bbDictionary_lookup(spawner->dict, key, &handle);
        spawn_function = spawner->spawn_graphics[handle.u64];

        spawn_function(file_line);
    }

    return bbSuccess;
}