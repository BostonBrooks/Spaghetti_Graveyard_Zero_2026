#ifndef BB_VP_PROJECTILE_SPAWNER_H
#define BB_VP_PROJECTILE_SPAWNER_H


#include "bbProjectile_data.h"
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

typedef struct bbViewportApp bbViewportApp;
extern I32 viewport_projectile_spawn_function_count;

typedef bbFlag bbVP_SpawnProjectile(bbViewportApp* viewport_app,
                                bbProjectile_data data,
                                bbHandle entity_handle,
                                bbHandle caster_entity_handle,
                                U64 random_seed);

typedef struct {
    bbVP_SpawnProjectile **spawn_functions;
    I32 spawn_function_count;
    bbDictionary *spawn_function_dict;
} bbVP_ProjectileSpawner;


bbFlag bbVP_ProjectileSpawner_init(bbVP_ProjectileSpawner *spawner);

bbFlag bbVP_ProjectileSpawner_add(bbVP_ProjectileSpawner *spawner,
                                   bbVP_SpawnProjectile *spawn_function,
                                  char *key);

I32 bbVP_ProjectileSpawner_lookupInt(bbVP_ProjectileSpawner *spawner,
                                     char *key);

bbFlag bbVP_ProjectileSpawner_populate(bbVP_ProjectileSpawner *viewport_spawner);

bbFlag bbVP_ProjectileSpawner_spawn(bbViewportApp *viewport,
                                    bbVP_ProjectileSpawner *spawner,
                                    U32 type,
                                    bbProjectile_data data,
                                    bbHandle entity_handle,
                                    bbHandle caster_entity_handle,
                                    U64 random_seed);

#endif //BB_VP_PROJECTILE_SPAWNER_H
