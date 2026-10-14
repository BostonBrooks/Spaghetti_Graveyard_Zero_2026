#ifndef BB_ECS_PROJ
#define BB_ECS_PROJ



#define NUM_PROJECTILES 1000
#include "engine/ECS/bbECS.h"

typedef struct {

    bbMapCoords start_coords;
    bbMapCoords current_coords;
    bbMapCoords end_coords;
    bbTime start_time;
    bbTime end_time;
    U64 gravity;
} bbProj_data;


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



bbFlag bbCI_updateProjectiles(bbCore* core,
                                  bbInstruction_source source, bbHandle action, bbTime time);

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