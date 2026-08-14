#ifndef LIVE_SPAWN_H
#define LIVE_SPAWN_H
#include "../../../../../engine/ECS/entity_spawner/bbEntitySpawner.h"


bbFlag bbCI_live_spawnEntity(bbCore* core, bbSpawnFunctionArgs args, char* key, bbInstruction_source source, bbHandle action);
bbFlag bbI_live_spawnEntity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_live_unspawnEntity_fn(bbCore* core, bbInstruction* instruction);


#endif //LIVE_SPAWN_H