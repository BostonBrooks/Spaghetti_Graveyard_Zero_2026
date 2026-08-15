#ifndef BB_GRAPHICS_SYSTEM_H
#define BB_GRAPHICS_SYSTEM_H


#include "engine/ECS/ECS.h"

typedef struct
{
    bbComponent component;
} bbGraphicsComponent;


typedef struct
{
    bbSystem system;
} bbGraphicsSystem;

bbFlag bbGraphicsSystem_init(bbGraphicsSystem* graphics_system, bbECS* ECS);

bbFlag bbCoreInput_spawnGraphicsComponent(bbCore* core,
                                          char* type,
                                          bbMapCoords MC,
                                          I32 drawable_state,
                                          bbHandle entity,
                                          bbHandle moveable,
                                          bbInstruction_source source,
                                          bbHandle action);

bbFlag bbCoreSynchronous_spawnGraphicsComponent(bbCore* core,
                                   bbMapCoords MC,
                                   bbHandle entity,
                                   bbHandle moveable,
                                   bbInstruction_source source);

bbFlag bbInstruction_spawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unspawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction);




#endif //BB_GRAPHICS_SYSTEM_H