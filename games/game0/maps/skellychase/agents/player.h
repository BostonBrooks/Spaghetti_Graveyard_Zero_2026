#include "engine/agents/bbAgentFunctions.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/network/bbNetworkApp.h"

//typedef bbFlag bbAgent_Command (bbAgent* agent,
//                                   bbAgentCommandType type,
//                                   bbAgentCommandData data);

bbFlag bbAgent_Command_Player(bbAgent* agent,bbAgentCommandType type,bbAgentCommandData data)
{
    bbCoreInput_setMoveableType(&home.core.core,0, agent->moveable, data,
                                     bbInstructionSource_internal,no_handle);
    bbCore_react(&home.core.core);

    return bbSuccess;
}