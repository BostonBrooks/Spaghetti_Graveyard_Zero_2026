#include "engine/agents/bbAgentFunctions.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/network/bbNetworkApp.h"

typedef bbFlag bbAgent_Command (bbAgent2* agent,
                                bbAgentCommandType type,
                                bbHandle data);

bbFlag bbAgent_Command_Player(bbAgent2* agent,bbAgentCommandType type,bbHandle data)
{
    bbDebug("agent->moveable = %d\n", agent->moveable);
    bbMapCoords* MC = data.ptr;

    //home.agents_app.agents.agents[0].goalpoint = *MC;

    bbCoreInput_setGoalPosition(&home.core.core,0, agent->moveable,*MC,bbInstructionSource_internal,no_handle);
    return bbSuccess;
}