
#include "engine/entities/bbAgentFunctions.h"
#include "engine/entities/bbMovables.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "games/game0/maps/systems_test/agents/player.h"
#include "games/game0/maps/systems_test/agents/fireball.h"
#include "games/game0/maps/systems_test/agents/skelly.h"




bbFlag bbAgentFunctions_populate(bbAgentFunctions* self)
{
    bbAgentFunctions_add(self, AgentUpdate, bbAgent_Update_Skelly2,"UPDATE_SKELLY");
    bbAgentFunctions_add(self, AgentUpdate, bbAgent_Update_Player,"UPDATE_PLAYER");
    bbAgentFunctions_add(self, AgentUpdate, bbAgent_Update_Fireball,"UPDATE_FIREBALL");
    bbAgentFunctions_add(self, AgentCommand, bbAgent_Command_Player,"COMMAND_PLAYER");
    bbAgentFunctions_add(self, AgentCommand, bbAgent_Command_Skelly,"COMMAND_SKELLY");
    return bbSuccess;
}


