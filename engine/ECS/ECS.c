#include "engine/ECS/ECS.h"

#include "engine/logic/bbBloatedPool.h"

bbFlag bbECS_init(bbECS* ECS)
{
    bbVPool_newBloated(&ECS->pool, sizeof(bbECS_entity), 1000, 1000, "ECS");
    bbList_init(&ECS->list, ECS->pool, NULL, offsetof(bbECS_entity, list_element_handle),NULL);
    return bbSuccess;
}

