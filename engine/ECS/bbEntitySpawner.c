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


bbFlag bbUnspawnFunction_add(bbEntitySpawner* spawner, bbUnspawnFunction* unspawn_function, char* key );