///The spawner has two parts, one that spawns entities in the main thread and
///one that spawns entities in the graphics thread. Both spawners read from the
///same file, so entities are spawned in pairs. An entity in the main thread will
///know how to send a message to an entity in the graphics thread and vice-versa.

#ifndef BB_SPAWNER_H
#define BB_SPAWNER_H


#include "engine/logic/bbFlag.h"
#include "../logic/bbDictionary.h"
#include "../logic/bbIntTypes.h"
typedef bbFlag bbParseFunction (char* string);

typedef bbFlag bbSpawnFunction (...);

typedef struct
{
    bbParseFunction** parse_core;
    bbParseFunction** parse_graphics;
    I32 parse_functions_available;
    bbDictionary* parse_dict;

    bbSpawnFunction** spawn_core;
    bbSpawnFunction** spawn_graphics;
    I32 spawn_functions_available;
    bbDictionary* spawn_dict;

} bbSpawner;


///Get ready to load spawn functions into memory
bbFlag bbSpawner_init(bbSpawner* spawner, I32 num_parsers, I32 num_entity_types);

///Load one pair of functions into memory
bbFlag bbParseFunction_add(bbSpawner* spawner,
    bbParseFunction* spawn_core, bbParseFunction* spawn_graphics, char* key );

bbFlag bbSpawnFunction_add(bbSpawner* spawner,
    bbSpawnFunction* spawn_core, bbSpawnFunction* spawn_graphics, char* key );

///Load spawn functions into memory
bbFlag bbSpawner_populate(bbSpawner* spawner);

bbFlag bbSpawner_getCore(bbSpawner* spawner,
    bbParseFunction** function, char* key);

bbFlag bbSpawner_getGraphics(bbSpawner* spawner,
    bbParseFunction** function, char* key);

bbFlag bbSpawner_spawnCore(bbSpawner* spawner, char* file_name);
bbFlag bbSpawner_spawnGraphics(bbSpawner* spawner, char* file_name);

#endif //BB_SPAWNER_H