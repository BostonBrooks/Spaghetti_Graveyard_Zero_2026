#ifndef BB_SERVER_SPAWNER_H
#define BB_SERVER_SPAWNER_H
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbFlag.h"

typedef bbFlag bbServer_SpawnFunction (char* string);

typedef struct
{
    bbServer_SpawnFunction** spawn_server;
    I32 spawn_functions_available;
    bbDictionary* dict;

} bbServer_Spawner;

///Get ready to load spawn functions into memory
bbFlag bbServer_Spawner_init(bbServer_Spawner* spawner, I32 num);

///Load one  function into memory
bbFlag bbServer_Spawner_add(bbServer_Spawner* spawner, bbServer_SpawnFunction* spawn_server, char* key );

///Load spawn functions into memory
bbFlag bbServer_Spawner_populate(bbServer_Spawner* spawner);

bbFlag bbServer_Spawner_getFunction(bbServer_Spawner* spawner,
    bbServer_SpawnFunction** function, char* key);

bbFlag bbServer_Spawner_spawn(bbServer_Spawner* spawner, char* file_name);


#endif //BB_SERVER_SPAWNER_H