

#include <stdio.h>
#include <stdlib.h>

#include "engine/spawner/bbSpawner.h"

#include "engine/data/bbHome.h"
#include "engine/entities/bbEntities.h"
#include "engine/logic/bbPrime.h"
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbTerminal.h"


bbFlag bbSpawner_init(bbSpawner* spawner, I32 num_parsers, I32 num_entity_types)
{
    spawner->parse_core = calloc(num_parsers, sizeof(bbParseFunction*));
    spawner->parse_graphics = calloc(num_parsers, sizeof(bbParseFunction*));
    spawner->parse_functions_available = 0;
    bbDictionary_new(&spawner->parse_dict, nextPrime(num_parsers));

    spawner->spawn_core = calloc(num_entity_types, sizeof(bbSpawnFunction*));
    spawner->spawn_graphics = calloc(num_entity_types, sizeof(bbSpawnFunction*));
    spawner->spawn_functions_available = 0;
    bbDictionary_new(&spawner->spawn_dict, nextPrime(num_entity_types));

    spawner->entity_new = calloc(num_entity_types, sizeof(bbEntity_new*));
    spawner->unit_new = calloc(num_entity_types, sizeof(bbUIUnit_new*));
    spawner->entity_new_functions_available = 0;
    bbDictionary_new(&spawner->entity_new_dict, nextPrime(num_entity_types));
    return bbSuccess;
}

bbFlag bbEntityFunction_add(bbSpawner* spawner,
    bbEntity_new* new_entity, bbUIUnit_new* new_unit, char* key )
{

    U32 available = spawner->entity_new_functions_available++;
    spawner->entity_new[available] = new_entity;
    spawner->unit_new[available] = new_unit;
    bbHandle handle;
    handle.u64 = available;

    bbDictionary_add(spawner->entity_new_dict, key, handle);

    return bbSuccess;
}

bbFlag bbParseFunction_add(bbSpawner* spawner,
    bbParseFunction* spawn_core, bbParseFunction* spawn_graphics, char* key )
{

    U32 available = spawner->parse_functions_available++;
    spawner->parse_core[available] = spawn_core;
    spawner->parse_graphics[available] = spawn_graphics;
    bbHandle handle;
    handle.u64 = available;

    bbDictionary_add(spawner->parse_dict, key, handle);

    return bbSuccess;
}

bbFlag bbSpawnFunction_add(bbSpawner* spawner,
    bbSpawnFunction* spawn_core, bbSpawnFunction* spawn_graphics, char* key )
{

    U32 available = spawner->spawn_functions_available++;
    spawner->spawn_core[available] = spawn_core;
    spawner->spawn_graphics[available] = spawn_graphics;
    bbHandle handle;
    handle.u64 = available;

    bbDictionary_add(spawner->spawn_dict, key, handle);

    return bbSuccess;
}


bbFlag bbSpawner_getCore(bbSpawner* spawner,
    bbParseFunction** function, char* key)
{
    bbHandle handle;
    bbDictionary_lookup(spawner->parse_dict, key, &handle);
    *function = spawner->parse_core[handle.u64];
    return bbSuccess;
}


bbFlag bbSpawner_getGraphics(bbSpawner* spawner,
    bbParseFunction** function, char* key)
{
    bbHandle handle;
    bbDictionary_lookup(spawner->parse_dict, key, &handle);
    *function = spawner->parse_graphics[handle.u64];
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

        bbParseFunction* spawn_function;
        bbHandle handle;
        bbDictionary_lookup(spawner->parse_dict, key, &handle);
        spawn_function = spawner->parse_core[handle.u64];

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

        bbParseFunction* spawn_function;
        bbHandle handle;
        bbDictionary_lookup(spawner->parse_dict, key, &handle);
        spawn_function = spawner->parse_graphics[handle.u64];

        spawn_function(file_line);
    }

    return bbSuccess;
}


bbFlag bbSpawner_spawnEntity(bbSpawner* spawner, bbMapCoords MC, I32 moveable_index, I32 entity_index, char* key)
{
    bbHandle function_handle;
    bbDictionary_lookup(spawner->entity_new_dict, key, &function_handle);
    bbEntity_new* function;
    function = spawner->entity_new[function_handle.u64];
    return function(MC, moveable_index, entity_index);
}

bbFlag bbUIUnit_newUnit(I32 type_index, bbMapCoords MC, I32 moveable_index, I32 entity_index)
{
    bbUIUnit_new* function;
    function = home.spawner.unit_new[type_index];
    return function(MC, moveable_index, entity_index);
}