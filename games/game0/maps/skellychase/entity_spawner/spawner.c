#include "engine/ECS/bbEntitySpawner.h"

I32 no_parse_functions = 193;
I32 no_spawn_functions = 194;
I32 no_unspawn_functions = 195;

bbFlag bbPF_null(void* spawner, char* string)
{
    printf("%s\n", string);
    return bbSuccess;
}

bbFlag bbPF_standard(void* spawner, char* string)
{
    char key[KEY_LENGTH];
    char entity_type[KEY_LENGTH];
    bbMapCoords position;
    bbMapCoords goalpoint;
    bbHandle server_handle;
    char spawn_functions[256];
    sscanf(string, "%[^','],%[^','],%d,%d,%d,%d,%d,%d,%d,%d,%[^'\n']\n",
        key,entity_type,&position.i,&position.j,&position.k,
        &goalpoint.i,&goalpoint.j,&goalpoint.k,&server_handle.bloated.index,
        &server_handle.bloated.collision,spawn_functions);
    bbDebug("%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
    key,entity_type,position.i,position.j,position.k,
    goalpoint.i,goalpoint.j,goalpoint.k,server_handle.bloated.index,
    server_handle.bloated.collision,spawn_functions);
    return bbSuccess;
}

bbFlag bbSF_null(void* spawner,
                               bbMapCoords position,
                               bbMapCoords goalpoint,
                               bbHandle server_handle,
                               bbInstruction_source source)
{
    bbDebug("position = (%d, %d, %d)\n"
            "goalpoint = (%d, %d, %d)\n"
            "server_handle = (%d, %d)\n"
            "source = %d\n",
            position.i, position.j, position.k,
            goalpoint.i, goalpoint.j, goalpoint.k,
            server_handle.bloated.index, server_handle.bloated.collision,
            source);
}

bbFlag bbEntitySpawner_populate(bbEntitySpawner* spawner)
{
    bbParseFunction_add(spawner, bbPF_null, "NULL");
    bbParseFunction_add(spawner, bbPF_standard, "STANDARD");

    bbSpawnFunction_add(spawner, bbSF_null, "NULL");
    return bbSuccess;
}