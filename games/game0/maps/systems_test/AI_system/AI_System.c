#include "engine/ECS/AI_system/bbAI_System.h"
#include "engine/logic/bbIntTypes.h"
#include "ai_null.h"

I32 ai_update_function_count = 193;
I32 ai_command_function_count = 194;

bbFlag bbAI_Update_Chase(bbAI_Component* component)
{
    //bbHere()
    bbMoveable* moveable;
    bbHandle moveable_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI,(bbComponent*)component, bbECS_Moveables,&moveable_handle, (bbComponent**)&moveable);

    if (moveable->type != bbMoveableType_Moving) return bbSuccess;

    bbMoveable* player_moveable;
    bbHandle player_handle;

    bbHandle_mapComponent(home.ECS.ECS, bbECS_ECS,home.ECS.ECS->player_character, bbECS_Moveables,&player_handle, (bbComponent**)&player_moveable);

    if (moveable_handle.bloated.index == player_handle.bloated.index) return bbSuccess;

    U64 distance_squared = (moveable->position.i - player_moveable->position.i)
                           *(moveable->position.i - player_moveable->position.i)
                           +(moveable->position.j - player_moveable->position.j)
                           *(moveable->position.j - player_moveable->position.j);

    if (distance_squared > POINTS_PER_TILE * POINTS_PER_TILE * 64) return bbSuccess;

    bbCI_Moveable_setGoalMovable(&home.core.core,moveable_handle,player_handle,bbInstructionSource_internal,no_handle);

    return bbSuccess;


}

bbFlag bbAI_Command_Player(bbAI_Component* component,
                         bbAI_CommandType type,
                         bbAI_CommandData data,
                         bool is_action)
{//is action: command is unable to modify state unless this function was called by a bbAction to ensure "core safeness"
    if (type == bbAI_setGoalPoint && is_action)
    {


        bbHandle moveable_handle;

        bbComponent_mapComponent(home.ECS.ECS,
            bbECS_AI,
            (bbComponent*)component,
            bbECS_Moveables,
            &moveable_handle,
            NULL);


        bbCS_Moveable_setGoalpoint(&home.core.core,
                                   moveable_handle,
                                   data.goal_point,
                                   bbInstructionSource_internal,
                                   no_handle);
    }

    if (type == bbAI_mapClick)
    {
        bbCoreInput_testClick3(&home.core.core, data.goal_point, home.core.core.actual_time, bbInstructionSource_internal, no_handle);
    }
    return bbSuccess;
}


bbFlag bbAI_Functions_populate(bbAI_Functions* self)
{
    // bbAgentFunctions_add(self, AgentUpdate, bbAgent_Update_Skelly2,"UPDATE_SKELLY");
    // bbAgentFunctions_add(self, AgentUpdate, bbAgent_Update_Player,"UPDATE_PLAYER");
    // bbAgentFunctions_add(self, AgentUpdate, bbAgent_Update_Fireball,"UPDATE_FIREBALL");
    // bbAgentFunctions_add(self, AgentCommand, bbAgent_Command_Player,"COMMAND_PLAYER");
    // bbAgentFunctions_add(self, AgentCommand, bbAgent_Command_Skelly,"COMMAND_SKELLY");


    bbAI_Functions_add(self,  AI_Update, bbAI_Update_NULL,"UPDATE_NULL" );
    bbAI_Functions_add(self,  AI_Update, bbAI_Update_Chase,"UPDATE_CHASE" );
    bbAI_Functions_add(self,  AI_Command, bbAI_Command_NULL,"COMMAND_NULL" );
    bbAI_Functions_add(self,  AI_Command, bbAI_Command_Player,"COMMAND_PLAYER" );
    return bbSuccess;
}


