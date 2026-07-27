#include "engine/ECS/bbEntitySpawner.h"

I32 no_parse_functions = 193;
I32 no_spawn_functions = 194;
I32 no_unspawn_functions = 195;

bbFlag bbPF_null(void* spawner, char* string)
{
    printf("%s\n", string);
    return bbSuccess;
}


bbFlag bbEntitySpawner_populate(bbEntitySpawner* spawner)
{
    bbParseFunction_add(spawner, bbPF_null, "NULL");

    return bbSuccess;
}