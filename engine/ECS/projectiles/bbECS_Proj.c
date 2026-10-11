#include "engine/ECS/projectiles/bbECS_Proj.h"

#include "engine/logic/bbSystemPool.h"

bbFlag bbProj_getComponent_fn(struct bbSystem* system, bbComponent** component, bbHandle component_handle);
bbFlag bbProj_getHandle_fn(struct bbSystem* system, bbComponent* component, bbHandle* component_handle);
bbFlag bbProj_deleteComponent_fn(struct bbSystem* system, bbHandle component_handle);




bbFlag bbProj_getComponent_fn(struct bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    return bbVPool_lookup(system->pool, (void**)component, component_handle);
}
bbFlag bbProj_getHandle_fn(struct bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    return bbVPool_reverseLookup(system->pool, (void*)component, component_handle);
}


bbFlag bbProj_system_init(bbProj_system* system,bbECS* ECS)
{
    bbVPool_newSystem(&system->system.pool,bbECS_AI, sizeof(bbProj_component),10,1000,"ENTITY AI");
    bbList_init(&system->list,system->system.pool,NULL,offsetof(bbProj_component,list),NULL,bbECS_Projectiles);

    system->system.getComponent = bbProj_getComponent_fn;
    system->system.getHandle = bbProj_getHandle_fn;
    system->system.delete = bbProj_deleteComponent_fn;

    system->system.ECS = ECS;
    ECS->systems[bbECS_Projectiles] = (bbSystem*)system;
    return bbSuccess;
}

bbFlag bbProj_system_deleteComponent_fn(struct bbSystem* system, bbHandle component_handle)
{
    bbProj_system* proj_system = (bbProj_system*)system;
    bbProj_component* component;
    bbProj_getComponent_fn(system, (bbComponent**)&component, component_handle);

    bbList_remove(&proj_system->list,component);
    bbVPool_free(proj_system->system.pool,component);

    return bbSuccess;
}