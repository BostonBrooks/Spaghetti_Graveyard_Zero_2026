
#ifndef GRAPHICS_SYSTEM
#define GRAPHICS_SYSTEM

#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"

bbFlag bbCoreInput_spawnGraphicsComponent(bbCore* core,
                                          char* type,
                                          bbMapCoords MC,
                                          bbHandle entity,
                                          bbHandle moveable,
                                          bbInstruction_source source,
                                          bbHandle action);

//bbInstruction_spawnGraphicsComponent is in instructions.h

bbFlag bbInstruction_spawnGraphicsComponent_fn(bbCore* core, bbInstruction* instruction);

//bbInstruction_unspawnGraphicsComponent is in instructions.h

#endif //GRAPHICS_SYSTEM