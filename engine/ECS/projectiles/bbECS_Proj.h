#ifndef BB_ECS_PROJ
#define BB_ECS_PROJ



#include "engine/core/bbInstruction.h"
#include "engine/ECS/bbECS.h"
#include "engine/ECS/projectiles/bbECS_Proj_data.h"




typedef struct {
    bbComponent component;
    bbListElement_Handle list;
    bbProj_data data;
} bbProj_component;

typedef struct {
    bbSystem system;
    bbList list;
    bbTime last_update;
} bbProj_system;

bbFlag bbProj_system_init(bbProj_system* system, bbECS* ECS, bbTime current_time );
bbFlag bbProj_system_interpolate(bbProj_system* system, bbECS* ECS, bbTime current_time );


bbFlag bbCI_updateProjectiles(bbCore *core, bbTime time,
                              bbInstruction_source source, bbHandle action);

bbFlag bbI_updateProjectiles_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unupdateProjectiles_fn(bbCore* core, bbInstruction* instruction);

///uses a bbProjectile on the stack to spawn a projectile component
bbFlag bbCI_spawnProjectile(bbCore* core,
                             bbHandle entity,
                             bbProj_data* data,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbCS_spawnProjectile(bbCore* core,
                             bbHandle entity,
                             bbProj_system** new_projectile,
                             bbProj_data* data,
                             bbInstruction_source source,
                             bbHandle action);


bbFlag bbI_spawnProjectile_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unspawnProjectile_fn(bbCore* core, bbInstruction* instruction);

// bbFlag bbProjectiles_getComponent(struct bbSystem* system, bbComponent** component, bbHandle component_handle);
// bbFlag bbProjectiles_getHandle(struct bbSystem* system, bbComponent* component, bbHandle* component_handle);

#endif //BB_PROJECITLES_H