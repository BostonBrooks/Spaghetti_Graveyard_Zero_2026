#include "engine/ECS/bbEntitySpawner.h"

bbFlag bbEntitySpawner_init(bbEntitySpawner* entity_spawner)
{
    entity_spawner->parse_functions = calloc(no_parse_functions,sizeof(bbParseFunction*));
    entity_spawner->parse_function_count = 0;
    bbDictionary_new(&entity_spawner->parse_dict, no_parse_functions);

    entity_spawner->spawn_functions = calloc(no_parse_functions,sizeof(bbSpawnFunction*));
    entity_spawner->spawn_function_count = 0;
    bbDictionary_new(&entity_spawner->spawn_dict, no_spawn_functions);

    entity_spawner->unspawn_functions = calloc(no_unspawn_functions,sizeof(bbUnspawnFunction*));
    entity_spawner->unspawn_function_count = 0;
    bbDictionary_new(&entity_spawner->unspawn_dict, no_unspawn_functions);
}

bbFlag bbParseFunction_get(bbEntitySpawner* spawner, bbParseFunction** function, char* key)
{
    bbHandle handle;
    bbDictionary_lookup(spawner->parse_dict, key, &handle);
    *function = spawner->parse_functions[handle.u64];
    return bbSuccess;
}

bbFlag bbParseFunction_add(bbEntitySpawner* spawner, bbParseFunction* parse_function, char* key )
{

    U32 available = spawner->parse_function_count++;
    spawner->parse_functions[available] = parse_function;
    bbHandle handle;
    handle.u64 = available;
    bbDictionary_add(spawner->parse_dict, key, handle);
    return bbSuccess;
}

bbFlag bbSpawnFunction_add(bbEntitySpawner* spawner, bbSpawnFunction* spawn_function, char* key )
{

    U32 available = spawner->spawn_function_count++;
    spawner->spawn_functions[available] = spawn_function;
    bbHandle handle;
    handle.u64 = available;
    bbDictionary_add(spawner->spawn_dict, key, handle);
    return bbSuccess;
}

bbFlag bbUnspawnFunction_add(bbEntitySpawner* spawner, bbUnspawnFunction* unspawn_function, char* key )
{

    U32 available = spawner->unspawn_function_count++;
    spawner->unspawn_functions[available] = unspawn_function;
    bbHandle handle;
    handle.u64 = available;
    bbDictionary_add(spawner->unspawn_dict, key, handle);
    return bbSuccess;
}


bbFlag bbEntitySpawner_spawnFile(bbEntitySpawner* spawner, char* file_name)
{
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

            bbParseFunction* parse_function;
            bbHandle handle;
            bbDictionary_lookup(spawner->parse_dict, key, &handle);
            parse_function = spawner->parse_functions[handle.u64];

            parse_function(spawner, file_line);
        }

        return bbSuccess;
    }
}