#include "engine/agents/bbAgentFunctions.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/network/bbNetworkApp.h"

typedef bbFlag bbAgent_Command (bbAgent2* agent,
                                bbAgentCommandType type,
                                bbHandle data);

bbFlag bbAgent_Command_Player(bbAgent2* agent,bbAgentCommandType type,bbHandle data)
{
    bbHere()
}