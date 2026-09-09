#include "engine/ECS/spatial/bbSpatial.h"

#include "bbSpatial_query.h"
#include "engine/ECS/AI_system/bbAI_System.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbSystemPool.h"
I32 bbSpatial_getSquareIndex(I32 i, I32 j, I32 squares_i, I32 squares_j){

    if (i<0 || i>=squares_i) return -1;
    if (j<0 || j>=squares_j) return -1;


    return i + squares_i * j;
}


bbSpatialSquare* bbSpatial_getSquare(bbSpatial* spatial, I32 i, I32 j, I32 squares_i, I32 squares_j)
{
    bbSpatialSquare* square;
    I32 index = bbSpatial_getSquareIndex(i, j, squares_i, squares_j);
    if (index == -1)
    {
        square = &spatial->lost;
    } else square = &spatial->squares[index];

    return square;
}

bbFlag bbSpatial_getComponent_fn(struct bbSystem* system, bbComponent** component, bbHandle component_handle)
{
    return bbVPool_lookup(system->pool, (void**)component, component_handle);
}
bbFlag bbSpatial_getHandle_fn(struct bbSystem* system, bbComponent* component, bbHandle* component_handle)
{
    return bbVPool_reverseLookup(system->pool, (void*)component, component_handle);
}

bbFlag bbSpatial_init(bbSpatial* system,bbECS* ECS, I32 squares_i, I32 squares_j)
{
    bbVPool_newSystem(&system->system.pool, bbECS_Spatial, sizeof(bbSpatial_Component),10,1000,"SPATIAL");

    bbList_init(&system->master_list,system->system.pool,NULL,offsetof(bbSpatial_Component, master_list),NULL);

    system->system.getComponent = bbSpatial_getComponent_fn;
    system->system.getHandle = bbSpatial_getHandle_fn;

    ECS->systems[bbECS_Spatial] = (bbSystem*)system;

    system->squares_i = squares_i;
    system->squares_j = squares_j;

    system->squares = calloc(squares_i*squares_j,sizeof(bbSpatialSquare));

    bbSpatialSquare* spatial_square = &system->lost;
    spatial_square->coords.i = -1;
    spatial_square->coords.j = -1;
    spatial_square->coords.k = 0;
    bbList_init(&spatial_square->list,system->system.pool, NULL,offsetof
        (bbSpatial_Component, square_list), NULL);

    for (I32 i = 0; i < squares_i;i++){
        for (I32 j = 0; j < squares_j; j++){
            I32 n = bbSpatial_getSquareIndex(i, j, squares_i, squares_j);
            spatial_square = &system->squares[n];
            spatial_square->coords.i = i;
            spatial_square->coords.j = j;
            spatial_square->coords.k = 0;

            bbList_init(&spatial_square->list,system->system.pool, NULL,offsetof
                (bbSpatial_Component, square_list), NULL);
        }
    }
    return bbSuccess;
}
//
// typedef bbFlag bbListFunction(bbList* list, void* node, void* cl);
// map function to list, going left-to-right
// bbFlag bbList_mapL(bbList* list, bbListFunction* myFunc, void* cl);

typedef struct
{
    bbSpatial* system;
    bbECS* ECS;
}bbSpatial_unpdate_cl;

bbFlag bbSpatial_unpdateOnce(bbList* list, void* node, void* cl)
{
    bbSpatial_unpdate_cl* data = (bbSpatial_unpdate_cl*)cl;
    bbHandle moveable_handle;
    bbMoveable* moveable;
    bbFlag flag = bbComponent_mapComponent(data->ECS,bbECS_Spatial,node,bbECS_Moveables,
        &moveable_handle,(bbComponent**)&moveable);

    if (flag != bbSuccess)
    {
        bbDebug("Probable orphaned spatial component\n");
        return bbContinue;
    }

    bbSpatial_setPosition(node, data->system, moveable->position);
    return bbContinue;
}

bbFlag bbSpatial_update(bbSpatial* system,bbECS* ECS)
{
    bbSpatial_unpdate_cl data;
    data.system = system;
    data.ECS = ECS;

    bbList_mapL(&system->master_list,bbSpatial_unpdateOnce,(void*)&data);

    return bbSuccess;
}

bbFlag bbSpatial_setPosition(bbSpatial_Component* component, bbSpatial* spatial,
                              bbMapCoords MC)
{

    component->map_coords = MC;
    bbSquareCoords oldSC = bbMapCoords_getSquareCoords(component->map_coords);
    bbSquareCoords newSC = bbMapCoords_getSquareCoords(MC);

    if (oldSC.i == newSC.i && oldSC.j == newSC.j) return bbSuccess;


    bbSpatialSquare* newSquare= bbSpatial_getSquare(spatial,newSC.i, newSC.j, spatial->squares_i, spatial->squares_j);
    bbSpatialSquare* oldSquare= bbSpatial_getSquare(spatial,oldSC.i, oldSC.j, spatial->squares_i, spatial->squares_j);

    if (oldSquare == newSquare) return bbSuccess;

    bbList_remove(&oldSquare->list, component);
    bbList_pushL(&newSquare->list, component);


}

bbFlag bbCS_spawnSpatialComponent(bbCore* core,
                             bbECS* ECS,
                             bbHandle entity,
                             bbSpatial_Component** this,
                             bbMapCoords MC,
                             bbInstruction_source source,
                             bbHandle action)
{
    //Spawn undo instruction

    bbSpatial_Component* component;
    bbHandle component_handle;

    bbSpatial* spatial = (bbSpatial*)ECS->systems[bbECS_Spatial];
    bbList_alloc2(&spatial->master_list,(void**)&component, &component_handle);
    bbList_pushL(&spatial->master_list,component);
    bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
    bbSpatialSquare* square= bbSpatial_getSquare(spatial,SC.i, SC.j, spatial->squares_i, spatial->squares_j);

    bbList_pushL(&square->list, component);


    component->component.entity_handle = entity;
    bbCS_entity_setComponent(core,
                         ECS,
                         component->component.entity_handle,
                         component_handle,
                         bbECS_Spatial,
                         bbInstructionSource_internal,
                         no_handle);

    if (this != NULL) *this = component;
}

bbFlag bbCI_spawnSpatialComponent(bbCore* core,
                             bbHandle entity,
                             bbMapCoords MC,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbI_spawnSpatialComponent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unspawnSpatialComponent_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbSpatial_getComponent_fn(struct bbSystem* system, bbComponent** component, bbHandle component_handle);
bbFlag bbSpatial_getHandle_fn(struct bbSystem* system, bbComponent* component, bbHandle* component_handle);