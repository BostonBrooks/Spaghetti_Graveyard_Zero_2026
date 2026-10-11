#ifndef BB_PROJECITLES_H
#define BB_PROJECITLES_H



#define NUM_PROJECTILES 1000
#include "engine/ECS/bbECS.h"

typedef struct {
    bbComponent component;
    bbListElement_Handle list;
    bbMapCoords star_coords;
    bbMapCoords end_coords;
    bbTime start_time;
    bbTime end_time;
    U64 gravity;
} bbProjectile;

typedef struct {
    bbSystem system;
    bbList list;
} bbProjectiles;

bbFlag bbProjectiles_init(bbProjectiles* system, bbECS* ECS);
bbFlag bbProjectiles_update(bbProjectiles* system);

///uses a bbProjectile on the stack to spawn a projectile comonent
bbFlag bbCI_spawnProjectile(bbCore* core,
                             bbHandle entity,
                             bbProjectile* projectile,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbCS_spawnProjectile(bbCore* core,
                             bbHandle entity,
                             bbProjectile** new_projectile,
                             bbProjectile* projectile,
                             bbInstruction_source source,
                             bbHandle action);


bbFlag bbI_spawnProjectile_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unspawnProjectile_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbProjectiles_getComponent(struct bbSystem* system, bbComponent** component, bbHandle component_handle);
bbFlag bbProjectiles_getHandle(struct bbSystem* system, bbComponent* component, bbHandle* component_handle);


#endif //BB_PROJECITLES_H