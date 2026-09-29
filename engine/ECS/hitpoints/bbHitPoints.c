#include "engine/ECS/hitpoints/bbHitPoints.h"

#include "engine/logic/bbSystemPool.h"



bbFlag bbHitPoints_init(bbHitPoints* system,bbECS* ECS) {
    bbVPool_newSystem(&system->system.pool,bbECS_Hitpoints,sizeof(bbHitPoint),10,1000,"HITPOINTS");
    bbList_init(&system->list,system->system.pool,NULL,offsetof(bbHitPoint,list),NULL,bbECS_Hitpoints);

    system->system.getComponent = bbHitPoints_getComponent;
    system->system.getHandle = bbHitPoints_getHandle;
    system->system.delete = bbHitPoints_delete;

    system->system.ECS = ECS;
    ECS->systems[bbECS_Hitpoints] = (bbSystem*)system;
    return bbSuccess;
}
bbFlag bbHitPoints_getComponent(struct bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    return bbVPool_lookup(system->pool, (void**)component, component_handle);
}
bbFlag bbHitPoints_getHandle(struct bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    return bbVPool_reverseLookup(system->pool, (void*)component, component_handle);
}
bbFlag bbHitPoints_delete(struct bbSystem* system, bbHandle component_handle)
{
    bbHitPoints* hitpoints = (bbHitPoints*)system;
    bbHitPoint* component;
    bbHitPoints_getComponent(system, (bbComponent**)&component, component_handle);
    bbList_remove(&hitpoints->list,component);
    bbVPool_free(hitpoints->system.pool,component);

    return bbSuccess;
}

bbFlag bbCS_Hitpoints_spawn(bbCore* core,
                             bbECS* ECS,
                             bbHandle entity,
                             bbHitPoint** hitpoint,
                             I32 max_hitpoints,
                             bbInstruction_source source,
                             bbHandle action) {
    bbNotImplemented()
}

bbFlag bbCI_Hitpoints_spawn(bbCore* core,
                             bbECS* ECS,
                             bbHandle entity,
                             I32 max_hitpoints,
                             bbInstruction_source source,
                             bbHandle action) {
    bbNotImplemented()
}

bbFlag bbI_Hitpoints_spawn_fn(bbCore* core, bbInstruction* instruction) {
    bbNotImplemented()
}
bbFlag bbI_Hitpoints_unspawn_fn(bbCore* core, bbInstruction* instruction) {
    bbNotImplemented()
}

bbFlag bbCS_Hitpoints_damage (bbCore* core,
                          bbECS* ECS,
                          bbHandle entity,
                          bbHitPoints_damageType damageType,
                          I32 hitpoints,
                          bbInstruction_source source,
                          bbHandle action) {
    bbNotImplemented()
}

bbFlag bbCI_Hitpoints_damage (bbCore* core,
                          bbECS* ECS,
                          bbHandle entity,
                          bbHitPoints_damageType damageType,
                          I32 hitpoints,
                          bbInstruction_source source,
                          bbHandle action) {
    bbNotImplemented()
}

bbFlag bbI_Hitpoints_damage_fn(bbCore* core, bbInstruction* instruction) {
    bbNotImplemented()
}
bbFlag bbI_Hitpoints_undamage_fn(bbCore* core, bbInstruction* instruction) {
    bbNotImplemented()
}