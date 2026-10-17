#include "engine/viewport/bbVP_ProjectileSpawner.h"


bbFlag bbVP_ProjectileSpawner_init(bbVP_ProjectileSpawner *spawner)
{

    spawner->spawn_functions = calloc(viewport_projectile_spawn_function_count,sizeof(bbVP_SpawnProjectile*));
    spawner->spawn_function_count = 0;
    bbDictionary_new(&spawner->spawn_function_dict, viewport_projectile_spawn_function_count);

    return bbSuccess;
}

bbFlag bbVP_ProjectileSpawner_add(bbVP_ProjectileSpawner *spawner,
                                  bbVP_SpawnProjectile *spawn_function,
                                  char *key)
{
    U32 available = spawner->spawn_function_count++;
    spawner->spawn_functions[available] = spawn_function;
    bbHandle handle;
    handle.u64 = available;
    bbDictionary_add(spawner->spawn_function_dict, key, handle);
    return bbSuccess;
}

I32 bbVP_ProjectileSpawner_lookupInt(bbVP_ProjectileSpawner *spawner,
                                     char *key)
{
    bbHandle handle;
    bbDictionary_lookup(spawner->spawn_function_dict,
                        key,
                        &handle);

    return handle.u64;
}

//bbFlag bbVP_ProjectileSpawner_populate(bbVP_ProjectileSpawner *viewport_spawner);

bbFlag bbVP_ProjectileSpawner_spawn(bbViewportApp *viewport,
                                    bbVP_ProjectileSpawner *spawner,
                                    U32 type,
                                    bbProjectile_data data,
                                    bbHandle entity_handle,
                                    bbHandle caster_entity_handle,
                                    U64 random_seed)
{
    bbVP_SpawnProjectile* function = spawner->spawn_functions[type];
    function(viewport,
             data,
             entity_handle,
             caster_entity_handle,
             random_seed);

    return bbSuccess;
}