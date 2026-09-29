#ifndef BB_HIPOINTS_H
#define BB_HIPOINTS_H
#include "engine/ECS/bbECS.h"

typedef enum {
    bbHitPoints_raw,
} bbHitPoints_damageType;

///A hitpoints component
typedef struct {
    bbComponent component;
    bbListElement_Handle list;
    ///systems can only see the health of an entity as it was at the last checkpoint
    I32 prev_health;
    I32 current_health;
    I32 max_health;
} bbHitPoint;

///The hitpoint system
typedef struct {
    bbSystem system;
    bbList list;
} bbHitPoints;

bbFlag bbHitPoints_init(bbHitPoints* system,bbECS* ECS);

bbFlag bbCS_Hitpoints_spawn(bbCore* core,
                             bbECS* ECS,
                             bbHandle entity,
                             bbHitPoint** hitpoint,
                             I32 max_hitpoints,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbCI_Hitpoints_spawn(bbCore* core,
                             bbECS* ECS,
                             bbHandle entity,
                             I32 max_hitpoints,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbI_Hitpoints_spawn_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_Hitpoints_unspawn_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbCS_Hitpoints_damage (bbCore* core,
                          bbECS* ECS,
                          bbHandle entity,
                          bbHitPoints_damageType damageType,
                          I32 hitpoints,
                          bbInstruction_source source,
                          bbHandle action);

bbFlag bbCI_Hitpoints_damage (bbCore* core,
                          bbECS* ECS,
                          bbHandle entity,
                          bbHitPoints_damageType damageType,
                          I32 hitpoints,
                          bbInstruction_source source,
                          bbHandle action);

bbFlag bbI_Hitpoints_damage_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_Hitpoints_undamage_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbHitPoints_getComponent(struct bbSystem* system, bbComponent** component, bbHandle component_handle);
bbFlag bbHitPoints_getHandle(struct bbSystem* system, bbComponent* component, bbHandle* component_handle);
bbFlag bbHitPoints_delete(struct bbSystem* system, bbHandle component_handle);



#endif //BB_HIPOINTS_H