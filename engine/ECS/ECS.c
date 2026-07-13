#include "engine/ECS/ECS.h"

#include "engine/logic/bbBloatedPool.h"

U64 getMask(bbECS_systems system)
{
    return 1 << system;
}

bbFlag bbECS_init(bbECS* ECS)
{
    bbVPool_newBloated(&ECS->pool, sizeof(bbECS_entity), 1000, 1000, "ECS");
    bbList_init(&ECS->list, ECS->pool, NULL, offsetof(bbECS_entity, list_element_handle),NULL);
    return bbSuccess;
}



bbFlag bbECS_newEntity(bbECS* ECS, bbHandle* handle, bbECS_entity** entity)
{
    bbECS_entity* new_entity;
    bbVPool_alloc(ECS->pool, (void**)&new_entity);

    bbHandle new_handle;
    bbVPool_reverseLookup(ECS->pool,new_entity,&new_handle);

    new_entity->state = bbECS_alive;
    new_entity->has_component = 0;

    if (handle!=NULL) *handle = new_handle;
    if (entity!=NULL) *entity = new_entity;

    return bbSuccess;
}

bbFlag bbECS_entity_getComponent(bbECS* ECS, bbHandle* component_handle, bbHandle entity_handle, bbECS_systems system)
{
    U64 mask = getMask(system);
    bbECS_entity* entity;
    bbVPool_lookup(ECS->pool, (void**)&entity, entity_handle);

    if (mask && entity->has_component == 0)
    {
        *component_handle = ECS->pool->null;
        return bbFail;
    }

    *component_handle = entity->components[system];
    return bbSuccess;
}

bbFlag bbECS_entity_setComponent(bbECS* ECS, bbHandle component, bbHandle entity_handle, bbECS_systems system)
{
    U64 mask = getMask(system);
    bbECS_entity* entity;
    bbVPool_lookup(ECS->pool, (void**)&entity, entity_handle);

    if (mask && entity->has_component)
    {
        return bbFail;
    }
    entity->has_component &= mask;

    entity->components[system] = component;

    return bbSuccess;
}