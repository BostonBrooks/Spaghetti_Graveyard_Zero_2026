#include "engine/viewport/bbDrawables.h"
#include "engine/viewport/bbViewportApp.h"
#include "engine/data/bbHome.h"

I32 viewport_spawn_function_count = 12;

bbFlag bbViewportSpawnCat(bbViewportApp* viewport,
                          bbMapCoords MC,
                          bbHandle entity_handle,
                          bbHandle moveable_handle)
{
    bbDrawable* drawable;
    bbDrawable_newCat(&drawable,
                      viewport->drawables,
                      &home.UI.graphics,
                      MC);

    return bbSuccess;
}

bbFlag bbViewportSpawner_populate(bbViewportSpawner* viewport_spawner)
{
    bbViewportSpawner_add(viewport_spawner,
                          bbViewportSpawnCat,
                          "CAT");
}