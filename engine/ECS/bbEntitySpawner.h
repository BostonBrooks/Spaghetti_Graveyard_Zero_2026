#ifndef BB_ENTITYSPAWNER_H
#define BB_ENTITYSPAWNER_H

#include "engine/ECS/ECS.h"
#include "engine/logic/bbFlag.h"

///read one line of input file and spawn one entity
typedef bbFlag bbParseFunction(void* spawner, char* line);


typedef struct
{

} bbEntitySpawner;




///Prepare spawner for use in program
bbFlag bbEntitySpawner_init(bbEntitySpawner* entity_spawner);
///Load functions specific to this map
bbFlag bbEntitySpawner_populate(bbEntitySpawner* entity_spawner);
///Read file and spawn entities, before game starts
bbFlag bbEntitySpawner_spawnFile(bbEntitySpawner* entity_spawner, char* file_name);

///Take input and spawn one entity
bbFlag bbCoreSynchronous_spawnEntity(bbEntitySpawner* entity_spawner,
                                     bbECS_entity** entity,
                                     I32 type,
                                     bbMapCoords position,
                                     bbMapCoords goalpoint,
                                     bbHandle server_handle);


#endif //BB_ENTITYSPAWNER_H