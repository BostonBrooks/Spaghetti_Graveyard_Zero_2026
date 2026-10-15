#ifndef BB_ECS_PROJ
#define BB_ECS_PROJ



#include "engine/core/bbInstruction.h"
#include "engine/ECS/bbECS.h"
#include "engine/ECS/projectiles/bbProjectileMovement_data.h"




typedef struct {
    bbComponent component;
    bbListElement_Handle list;
    bbProjectileMovement_data data;
} bbProjectileMovement_component;

typedef struct {
    bbSystem system;
    bbList list;
    bbTime last_update;
} bbProjectileMovement;

bbFlag bbProjectileMovement_init(bbProjectileMovement* system, bbECS* ECS, bbTime current_time );
bbFlag bbProjectileMovement_interpolate(bbProjectileMovement* system, bbECS* ECS, bbTime current_time );


bbFlag bbCI_updateProjectileMovement(bbCore *core, bbTime time,
                              bbInstruction_source source, bbHandle action);

bbFlag bbI_updateProjectileMovement_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unupdateProjectileMovement_fn(bbCore* core, bbInstruction* instruction);

///uses a bbProjectile on the stack to spawn a projectile component
bbFlag bbCI_spawnProjectileMovement(bbCore* core,
                             bbHandle entity,
                             bbProjectileMovement_data* data,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbCS_spawnProjectileMovement(bbCore* core,
                             bbHandle entity,
                             bbProjectileMovement_component** new_projectile,
                             bbProjectileMovement_data* data,
                             bbInstruction_source source,
                             bbHandle action);


bbFlag bbI_spawnProjectileMovement_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unspawnProjectileMovement_fn(bbCore* core, bbInstruction* instruction);

// bbFlag bbProjectiles_getComponent(struct bbSystem* system, bbComponent** component, bbHandle component_handle);
// bbFlag bbProjectiles_getHandle(struct bbSystem* system, bbComponent* component, bbHandle* component_handle);

#endif //BB_PROJECITLES_H