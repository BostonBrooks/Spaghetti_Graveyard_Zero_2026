#ifndef COREINPUTS_H
#define COREINPUTS_H

#include "engine/agents/bbAgents2.h"
#include "engine/core/bbCore.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbHandle.h"
bbFlag bbCoreInput_updateAgentsSquare(bbCore* core, bbAgents2* agents);

bbFlag bbCoreInput_updateAgentSquare(bbCore* core, bbHandle agent, bbSquareCoords SC);

#endif COREINPUTS_H