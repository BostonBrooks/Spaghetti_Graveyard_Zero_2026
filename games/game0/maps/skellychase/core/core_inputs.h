#ifndef COREINPUTS_H
#define COREINPUTS_H

#include "engine/entities/bbAgents.h"
#include "engine/core/bbCore.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbHandle.h"
#include "engine/core/bbInstruction.h"
bbFlag bbCoreInput_updateAgentsSquare(bbCore* core, bbAgents* agents,bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_updateAgentSquare(bbCore* core, bbHandle agent, bbSquareCoords SC,bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_updateAgents(bbCore* core, bbAgents* agents,bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_updateAgent(bbCore* core, bbHandle agent,bbInstruction_source source, bbHandle action);


bbFlag bbCoreInput_commandAgent_setGoalPoint(bbCore* core, bbHandle agent, bbMapCoords MC,bbInstruction_source source, bbHandle action);

bbFlag bbCoreInput_spawnUnitOut(bbCore* core, I32 unit_type, bbMapCoords MC,bbMapCoords MC2, U64 time,
bbInstruction_source source, bbHandle action);
bbFlag bbCoreInput_spawnUnitIn(bbCore* core, bbMapCoords MC,bbMapCoords goalcoords,I32 type_index, I32 entity_index,
    I32 moveable_index, U64 time, bbInstruction_source source, bbHandle action);

#endif //COREINPUTS_H