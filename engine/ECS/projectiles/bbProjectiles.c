#include "engine/ECS/projectiles/bbProjectiles.h"

#include "engine/logic/bbSystemPool.h"

bbFlag bbProjectiles_getComponent_fn(struct bbSystem* system, bbComponent** component, bbHandle component_handle);
bbFlag bbProjectiles_getHandle_fn(struct bbSystem* system, bbComponent* component, bbHandle* component_handle);
bbFlag bbProjectiles_deleteComponent_fn(struct bbSystem* system, bbHandle component_handle);




bbFlag bbProjectiles_getComponent_fn(struct bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    return bbVPool_lookup(system->pool, (void**)component, component_handle);
}
bbFlag bbProjectiles_getHandle_fn(struct bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    return bbVPool_reverseLookup(system->pool, (void*)component, component_handle);
}


bbFlag bbProjectiles_init(bbProjectiles* system,bbECS* ECS)
{
    bbVPool_newSystem(&system->system.pool,bbECS_AI, sizeof(bbProjectile),10,1000,"ENTITY AI");
    bbList_init(&system->list,system->system.pool,NULL,offsetof(bbProjectile,list),NULL,bbECS_Projectiles);

    system->system.getComponent = bbProjectiles_getComponent_fn;
    system->system.getHandle = bbProjectiles_getHandle_fn;
    system->system.delete = bbProjectiles_deleteComponent_fn;

    system->system.ECS = ECS;
    ECS->systems[bbECS_Projectiles] = (bbSystem*)system;
    return bbSuccess;
}

bbFlag bbProjectiles_deleteComponent_fn(struct bbSystem* system, bbHandle component_handle)
{
    bbProjectiles* AI_System = (bbProjectiles*)system;
    bbProjectile* component;
    bbProjectiles_getComponent_fn(system, (bbComponent**)&component, component_handle);

    bbList_remove(&AI_System->list,component);
    bbVPool_free(AI_System->system.pool,component);

    return bbSuccess;
}