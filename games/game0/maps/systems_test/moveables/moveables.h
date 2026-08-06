#ifndef MOVEABLES_H
#define MOVEABLES_H
#include "engine/ECS/moveables/bbMoveables.h"

bbFlag bbCoreInput_updateMoveables(bbCore* core, bbInstruction_source source, bbHandle action);
bbFlag bbInstruction_updateMoveables_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unupdateMoveables_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbCoreSynchronous_spawnTestMoveable(bbCore* core,
                                           bbHandle ECS_entity_handle,
                                           bbHandle* moveable_handle,
                                           bbMapCoords position,
                                           bbInstruction_source source,
                                           bbHandle action);

bbFlag bbInstruction_spawnTestMoveable_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unspawnTestMoveable_fn(bbCore* core, bbInstruction* instruction);


#endif //MOVEABLES_H