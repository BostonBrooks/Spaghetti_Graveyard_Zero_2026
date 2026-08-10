#include "engine/ECS/bbAI_System.h"
#include "engine/logic/bbIntTypes.h"
#include "ai_null.h"

I32 ai_update_function_count = 193;
I32 ai_command_function_count = 194;


bbFlag bbAI_Functions_populate(bbAI_Functions* self)
{
    // bbAgentFunctions_add(self, AgentUpdate, bbAgent_Update_Skelly2,"UPDATE_SKELLY");
    // bbAgentFunctions_add(self, AgentUpdate, bbAgent_Update_Player,"UPDATE_PLAYER");
    // bbAgentFunctions_add(self, AgentUpdate, bbAgent_Update_Fireball,"UPDATE_FIREBALL");
    // bbAgentFunctions_add(self, AgentCommand, bbAgent_Command_Player,"COMMAND_PLAYER");
    // bbAgentFunctions_add(self, AgentCommand, bbAgent_Command_Skelly,"COMMAND_SKELLY");


    bbAI_Functions_add(self,  AI_Update, bbAI_Update_NULL,"UPDATE_NULL" );
    bbAI_Functions_add(self,  AI_Command, bbAI_Command_NULL,"COMMAND_NULL" );
    return bbSuccess;
}