#ifndef BB_VIEWPORTSPAWNER_H
#define BB_VIEWPORTSPAWNER_H


#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

typedef struct bbViewportApp bbViewportApp;
extern I32 viewport_spawn_function_count;

typedef bbFlag bbViewportSpawnFunction(bbViewportApp* viewport,
                                       bbMapCoords MC,
                                       bbHandle entity_handle,
                                       bbHandle moveable_handle);

typedef struct
{
    bbViewportSpawnFunction** spawn_functions;
    I32 spawn_function_count;
    bbDictionary* spawn_function_dict;
} bbViewportSpawner;


bbFlag bbViewportSpawner_init(bbViewportSpawner* viewport_spawner);

bbFlag bbViewportSpawner_add(bbViewportSpawner* spawner,
                             bbViewportSpawnFunction* spawn_function,
                             char* key );

I32 bbViewportSpawner_lookupInt(bbViewportSpawner* spawner,
                                char* key );

bbFlag bbViewportSpawner_populate(bbViewportSpawner* viewport_spawner);

bbFlag bbViewportSpawner_spawnDrawable(bbViewportApp* viewport,
                                     bbViewportSpawner* spawner,
                                     U32 type,
                                     bbMapCoords MC,
                                     bbHandle entity_handle,
                                     bbHandle moveable_handle);

#endif //BB_VIEWPORTSPAWNER_H