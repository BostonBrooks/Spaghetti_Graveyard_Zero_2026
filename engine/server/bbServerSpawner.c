
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/server/bbServerSpawner.h"

#include <stdio.h>
#include <stdlib.h>

#include "engine/logic/bbPrime.h"
#include "engine/logic/bbTerminal.h"

bbFlag bbServer_Spawner_init(bbServer_Spawner* spawner, I32 num)
{
    spawner->spawn_server = calloc(num, sizeof (bbServer_SpawnFunction*));
    spawner->spawn_functions_available = 0;
    bbDictionary_new(&spawner->dict, nextPrime(num));
    return bbSuccess;
}

bbFlag bbServer_Spawner_add(bbServer_Spawner* spawner,
    bbServer_SpawnFunction* spawn_server, char* key )
{
    U32 available = spawner->spawn_functions_available++;
    spawner->spawn_server[available] = spawn_server;
    bbHandle handle;
    handle.u64 = available;

    bbDictionary_add(spawner->dict, key, handle);

    return bbSuccess;
}

bbFlag bbServer_Spawner_getFunction(bbServer_Spawner* spawner,
    bbServer_SpawnFunction** function, char* key)
{
    bbHandle handle;
    bbDictionary_lookup(spawner->dict, key, &handle);
    *function = spawner->spawn_server[handle.u64];
    return bbSuccess;
}


bbFlag bbServer_Spawner_spawn(bbServer_Spawner* spawner, char* file_name)
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

        bbServer_SpawnFunction* spawn_function;
        bbHandle handle;
        bbDictionary_lookup(spawner->dict, key, &handle);
        spawn_function = spawner->spawn_server[handle.u64];

        spawn_function(file_line);
    }

    return bbSuccess;
}

bbFlag bbSF_null(char* string)
{
    printf("%s", string);
    return bbSuccess;
}

bbFlag bbServer_Spawner_populate(bbServer_Spawner* spawner)
{
    bbServer_Spawner_add(spawner,bbSF_null, "NULL");
    bbServer_Spawner_add(spawner,bbSF_null, "TREE");
    bbServer_Spawner_add(spawner,bbSF_null, "SKELETON");
    bbServer_Spawner_add(spawner,bbSF_null, "SKELLY");
    bbServer_Spawner_add(spawner,bbSF_null, "ZOMBIE");
    return bbSuccess;
}
