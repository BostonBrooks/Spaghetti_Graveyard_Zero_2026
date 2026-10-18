#include "games/game0/maps/systems_test/viewport_spawner/arrow.h"
#include "games/game0/maps/systems_test/viewport_spawner/arrows.h"
#include "engine/viewport/bbViewportApp.h"

I32 viewport_projectile_spawn_function_count = 12;


bbFlag bbVP_ProjectileSpawner_populate(bbVP_ProjectileSpawner *viewport_spawner)
{
    bbVP_ProjectileSpawner_add(viewport_spawner,
                          bbProjectile_spawnArrow,
                          "ARROW");

    bbVP_ProjectileSpawner_add(viewport_spawner,
                      bbProjectile_spawnArrows,
                      "ARROWS");

    return bbSuccess;
}