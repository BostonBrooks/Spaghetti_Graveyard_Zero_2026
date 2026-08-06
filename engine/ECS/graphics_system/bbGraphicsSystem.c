
#include "engine/ECS/bbGraphicsSystem.h"

#include "engine/logic/bbBloatedPool.h"


bbFlag bbGraphicsSystem_getComponent_fn(bbSystem* system, bbComponent** component, bbHandle component_handle);
bbFlag bbGraphicsSystem_getHandle_fn(bbSystem* system, bbComponent* component, bbHandle* component_handle);


bbFlag bbGraphicsSystem_init(bbGraphicsSystem* graphics_system, bbECS* ECS)
{
    bbVPool_newBloated(&graphics_system->system.pool, sizeof(bbGraphicsComponent), 100, 100, "SERVER ENTITIES");

    graphics_system->system.getComponent = bbGraphicsSystem_getComponent_fn;
    graphics_system->system.getHandle = bbGraphicsSystem_getHandle_fn;


    ECS->systems[bbECS_Graphics] = (bbSystem* )graphics_system;
}


bbFlag bbGraphicsSystem_getComponent_fn(bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    return bbVPool_lookup(system->pool,(void**)component,component_handle);
}

bbFlag bbGraphicsSystem_getHandle_fn(bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    return bbVPool_reverseLookup(system->pool,(void*)component,component_handle);
}