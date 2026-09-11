#ifndef BB_SPATIAL_H
#define BB_SPATIAL_H


#include "engine/ECS/bbECS.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbList.h"


typedef struct
{
    bbSquareCoords coords;
    bbList list;
} bbSpatialSquare;

typedef struct
{
    bbSystem system;

    I32 squares_i;
    I32 squares_j;

    bbList master_list;
    bbSpatialSquare lost;
    bbSpatialSquare* squares;


} bbSpatial;


typedef struct
{
    bbComponent component;
    bbListElement_Handle master_list;
    bbListElement_Handle square_list;
    bbMapCoords map_coords;
    bbSquareCoords square_coords;
    //enum state flags?
} bbSpatial_Component;


bbFlag bbSpatial_init(bbSpatial* system,bbECS* ECS, I32 squares_i, I32 squares_j);
bbFlag bbSpatial_update(bbSpatial* system,bbECS* ECS);


I32 bbSpatial_getSquareIndex(I32 i, I32 j, I32 squares_i, I32 squares_j);

bbFlag bbSpatial_setPosition(bbSpatial_Component* self, bbSpatial* spatial,
                              bbMapCoords MC);

bbFlag bbCS_spawnSpatialComponent(bbCore* core,
                             bbECS* ECS,
                             bbHandle entity,
                             bbSpatial_Component** this,
                             bbMapCoords MC,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbCI_spawnSpatialComponent(bbCore* core,
                             bbHandle entity,
                             bbMapCoords MC,
                             bbInstruction_source source,
                             bbHandle action);

bbFlag bbI_spawnSpatialComponent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unspawnSpatialComponent_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbSpatial_getComponent_fn(struct bbSystem* system, bbComponent** component, bbHandle component_handle);
bbFlag bbSpatial_getHandle_fn(struct bbSystem* system, bbComponent* component, bbHandle* component_handle);
#endif //BB_SPATIAL_H