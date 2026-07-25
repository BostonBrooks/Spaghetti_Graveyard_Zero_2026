#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "engine/ECS/ECS.h"

//Component
typedef struct
{
    bbHandle bbECS_entity_handle;
} bbGraphicsComponent;

//System
typedef struct
{
    bbSystem system;

} bbGraphicsSystem;

bbFlag bbGraphicsSystem_new(bbGraphicsSystem** graphics_system);

bbFlag bbCoreInput_spawnGraphicsComponent(bbCore* core,
                                   bbMapCoords MC,
                                   bbHandle entity,
                                   bbHandle moveable,
                                   bbInstruction_source source,
                                   bbHandle action);

bbFlag bbInstruction_spawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unspawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction);


#endif //GRAPHICS_SYSTEM_H
