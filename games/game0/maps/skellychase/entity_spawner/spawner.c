#include "engine/ECS/bbEntitySpawner.h"

I32 no_parse_functions = 193;
I32 no_spawn_functions = 194;
I32 no_unspawn_functions = 195;

bbFlag bbPF_null(char* string)
{
    bbDebug("%s\n", string);
    return bbSuccess;
}
