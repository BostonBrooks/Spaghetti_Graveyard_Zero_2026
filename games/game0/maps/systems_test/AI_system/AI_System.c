#include "ai_instructions.h"
#include "engine/ECS/AI_system/bbAI_System.h"
#include "engine/logic/bbIntTypes.h"
#include "ai_null.h"
#include "games/game0/maps/systems_test/core/player_goalpoint.h"

I32 ai_update_function_count = 193;
I32 ai_command_function_count = 194;

bbFlag bbAI_Update_Chase(bbAI_Component* component)
{
    //bbHere()
    bbMoveable* moveable;
    bbHandle moveable_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_Moveables, &moveable_handle,
                             (bbComponent**)&moveable);


    bbMoveable* player_moveable;
    bbHandle player_handle;

    bbHandle_mapComponent(home.ECS.ECS, bbECS_ECS,
                          home.ECS.ECS->player_character, bbECS_Moveables,
                          &player_handle, (bbComponent**)&player_moveable);

    if (moveable_handle.bloated.index == player_handle.bloated.index)
        return bbSuccess;

    U64 distance_squared = (moveable->position.i - player_moveable->position.i)
        * (moveable->position.i - player_moveable->position.i)
        + (moveable->position.j - player_moveable->position.j)
        * (moveable->position.j - player_moveable->position.j);

    if (moveable->type == bbMoveableType_Idle)
    {
        if (distance_squared > POINTS_PER_TILE * POINTS_PER_TILE * 200)
            return bbSuccess;

        if (component->last_state_change > home.core.core.simulation_time - 60) return bbSuccess;

        bbCI_Moveable_setGoalMovable(&home.core.core, moveable_handle,
                                     player_handle,
                                     bbInstructionSource_internal, no_handle);


    }



    if (moveable->type == bbMoveableType_Following)
    {
        if (distance_squared < POINTS_PER_TILE * POINTS_PER_TILE * 8) {
            bbCI_Moveable_setIdle(&home.core.core,
                             moveable_handle,
                             bbInstructionSource_internal, no_handle);

            bbHandle AI_handle;

            bbComponent_getHandle(&home.ECS.AI_system.system,(bbComponent*)component,&AI_handle);

            return bbSuccess;

        }

        if (distance_squared < POINTS_PER_TILE * POINTS_PER_TILE * 160) return
            bbSuccess;

        bbCI_Moveable_setIdle(&home.core.core,
                                     moveable_handle,
                                     bbInstructionSource_internal, no_handle);


    }
    return bbSuccess;
}

bbFlag bbAI_Update_Striking(bbAI_Component* component)
{
    //bbHere()

    bbHandle AI_handle;
    bbComponent_getHandle(&home.ECS.AI_system.system,(bbComponent*)component, &AI_handle);


    bbHandle entity_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_ECS, &entity_handle,
                             NULL);

    bbMoveable* moveable;
    bbHandle moveable_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_Moveables, &moveable_handle,
                             (bbComponent**)&moveable);


    bbMoveable* player_moveable;
    bbHandle player_handle;

    bbHandle_mapComponent(home.ECS.ECS, bbECS_ECS,
                          home.ECS.ECS->player_character, bbECS_Moveables,
                          &player_handle, (bbComponent**)&player_moveable);


    I64 delta_i = (player_moveable->position.i - moveable->position.i);
    I64 delta_j = (player_moveable->position.j - moveable->position.j);
    I64 distance = bbArith64_sqrt2(delta_i * delta_i + delta_j * delta_j);


    //bbAIState_Idle,
    //bbAIState_Approaching,
    //bbAIState_Striking,
    //bbAIState_Recovering,

    switch (component->state)
    {
    case bbAIState_Idle:
        {
            if (distance > POINTS_PER_TILE * 20) return bbSuccess;

            bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);

            bbCI_Moveable_setGoalMovable(&home.core.core, moveable_handle,
                             player_handle,
                             bbInstructionSource_internal, no_handle);


            bbCI_AI_setApproaching(&home.core.core,
                                   AI_handle,
                                   home.ECS.ECS->player_character,
                                   home.core.core.simulation_time,
                             bbInstructionSource_internal, no_handle);
            break;
        }
    case bbAIState_Approaching:
        {
            if (distance<POINTS_PER_TILE * 5)
            {
                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_idle);

                bbCI_Moveable_setIdle(&home.core.core,
                                 moveable_handle,
                                 bbInstructionSource_internal, no_handle);




                bbCI_AI_setStriking(&home.core.core,
                                       AI_handle,
                                       home.ECS.ECS->player_character,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }

            if (distance>POINTS_PER_TILE * 25)
            {
                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_idle);

                bbCI_Moveable_setIdle(&home.core.core,
                                 moveable_handle,
                                 bbInstructionSource_internal, no_handle);




                bbCI_AI_setIdle(&home.core.core,
                                       AI_handle,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }
            break;
        }
    case bbAIState_Striking:
        {
            if (component->last_state_change < home.core.core.simulation_time - 10)
            {

                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_idle);

                bbCI_Moveable_setIdle(&home.core.core,
                 moveable_handle,
                 bbInstructionSource_internal, no_handle);

                bbCI_AI_setRecovering(&home.core.core,
                                       AI_handle,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }

            break;
        }
    case bbAIState_Recovering:
        {
            if (component->last_state_change < home.core.core.simulation_time - 10)
            {
                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);

                bbCI_Moveable_setGoalMovable(&home.core.core, moveable_handle,
                                 player_handle,
                                 bbInstructionSource_internal, no_handle);

                bbCI_AI_setApproaching(&home.core.core,
                                       AI_handle,
                                       home.ECS.ECS->player_character,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }

            break;
        }
    }

    return bbSuccess;
}


bbFlag bbAI_Update_Lunging(bbAI_Component* component)
{
    //bbHere()

    bbHandle AI_handle;
    bbComponent_getHandle(&home.ECS.AI_system.system,(bbComponent*)component, &AI_handle);

    bbHandle entity_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_ECS, &entity_handle,
                             NULL);


    bbMoveable* moveable;
    bbHandle moveable_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_Moveables, &moveable_handle,
                             (bbComponent**)&moveable);


    bbMoveable* player_moveable;
    bbHandle player_handle;

    bbHandle_mapComponent(home.ECS.ECS, bbECS_ECS,
                          home.ECS.ECS->player_character, bbECS_Moveables,
                          &player_handle, (bbComponent**)&player_moveable);


    I64 delta_i = (player_moveable->position.i - moveable->position.i);
    I64 delta_j = (player_moveable->position.j - moveable->position.j);
    I64 distance = bbArith64_sqrt2(delta_i * delta_i + delta_j * delta_j);


    //bbAIState_Idle,
    //bbAIState_Approaching,
    //bbAIState_Striking,
    //bbAIState_Recovering,

    switch (component->state)
    {
    case bbAIState_Idle:
        {
            if (distance > POINTS_PER_TILE * 20) return bbSuccess;

            bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);

            bbCI_Moveable_setGoalMovable(&home.core.core, moveable_handle,
                             player_handle,
                             bbInstructionSource_internal, no_handle);


            bbCI_AI_setApproaching(&home.core.core,
                                   AI_handle,
                                   home.ECS.ECS->player_character,
                                   home.core.core.simulation_time,
                             bbInstructionSource_internal, no_handle);
            break;
        }
    case bbAIState_Approaching:
        {
            if (distance<POINTS_PER_TILE * 10)
            {
                bbCI_Moveable_setGoalLunging(&home.core.core,
                                 moveable_handle,
                                 player_handle,
                                 bbInstructionSource_internal, no_handle);

                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);


                bbCI_AI_setStriking(&home.core.core,
                                       AI_handle,
                                       home.ECS.ECS->player_character,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }

            if (distance>POINTS_PER_TILE * 25)
            {

                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_idle);

                bbCI_Moveable_setIdle(&home.core.core,
                                 moveable_handle,
                                 bbInstructionSource_internal, no_handle);




                bbCI_AI_setIdle(&home.core.core,
                                       AI_handle,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }
            break;
        }
    case bbAIState_Striking:
        {
            if (component->last_state_change < home.core.core.simulation_time - 20)
            {
                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_idle);

                bbCI_Moveable_setIdle(&home.core.core,
                 moveable_handle,
                 bbInstructionSource_internal, no_handle);

                bbCI_AI_setRecovering(&home.core.core,
                                       AI_handle,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }

            break;
        }
    case bbAIState_Recovering:
        {
            if (component->last_state_change < home.core.core.simulation_time - 30)
            {

                if (distance<POINTS_PER_TILE * 10)
                {

                    bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);

                    bbCI_Moveable_setGoalLunging(&home.core.core,
                                     moveable_handle,
                                     player_handle,
                                     bbInstructionSource_internal, no_handle);




                    bbCI_AI_setStriking(&home.core.core,
                                           AI_handle,
                                           home.ECS.ECS->player_character,
                                           home.core.core.simulation_time,
                                     bbInstructionSource_internal, no_handle);
                    break;
                }

                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);

                bbCI_Moveable_setGoalMovable(&home.core.core, moveable_handle,
                                 player_handle,
                                 bbInstructionSource_internal, no_handle);

                bbCI_AI_setApproaching(&home.core.core,
                                       AI_handle,
                                       home.ECS.ECS->player_character,
                                       home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);
            }

            break;
        }
    }

    return bbSuccess;
}


bbFlag bbAI_Update_Player(bbAI_Component* component)
{

    switch (component->state)
    {
    case bbAIState_Moving:
        {
            bbMoveable* moveable;
            bbHandle moveable_handle;
            bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                                     bbECS_Moveables, &moveable_handle,
                                     (bbComponent**)&moveable);

            I64 delta_i = moveable->goalpoint.i - moveable->position.i;
            I64 delta_j = moveable->goalpoint.j - moveable->position.j;

            I64 distance_squared = delta_i * delta_i + delta_j * delta_j;

            if (distance_squared<=POINTS_PER_PIXEL * POINTS_PER_PIXEL)
            {
                bbHandle entity_handle;

                bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                                         bbECS_ECS, &entity_handle,
                                         (bbComponent**)&moveable);

                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_idle);
                bbHandle AI_handle;

                bbComponent_getHandle(&home.ECS.AI_system.system,(bbComponent*)component,&AI_handle);

                bbCI_AI_setIdle(&home.core.core,
                             AI_handle,
                             home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);

                bbCI_Moveable_setIdle(&home.core.core,
                    moveable_handle,
                    bbInstructionSource_internal, no_handle);
            }


        }
    }
}

bbFlag bbAI_Command_Player(bbAI_Component* component,
                           bbAI_CommandType type,
                           bbAI_CommandData data,
                           bool is_action)
{


    //is action: command is unable to modify state unless this function was called by a bbAction to ensure "core safeness"
    //I need to be careful about the player modifying the state without sending a message to the server and back
    if (type == bbAI_setGoalPoint && is_action)
    {
        bbHandle entity_handle;
        bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                                 bbECS_ECS, &entity_handle,
                                 NULL);

        bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);

        bbHandle AI_handle;
        bbComponent_getHandle(&home.ECS.AI_system.system,(bbComponent*)component,&AI_handle);

        bbCI_AI_setMoving(&home.core.core,
             AI_handle,
             home.core.core.simulation_time,
                 bbInstructionSource_internal, no_handle);

        //TODO set state AI state moving

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
        if (data.integer == 0)
        {
            bbCoreInput_testClick3(&home.core.core, data.goal_point,
                                   home.core.core.actual_time,
                                   bbInstructionSource_internal, no_handle);
        }
        else
        {
            bbCoreInput_testClick4(&home.core.core, data.goal_point,
                                   home.core.core.actual_time,
                                   bbInstructionSource_internal, no_handle);
        }
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


    bbAI_Functions_add(self, AI_Update, bbAI_Update_NULL, "UPDATE_NULL");
    bbAI_Functions_add(self, AI_Update, bbAI_Update_Lunging, "UPDATE_LUNGING");
    bbAI_Functions_add(self, AI_Update, bbAI_Update_Striking, "UPDATE_STRIKING");
    bbAI_Functions_add(self, AI_Update, bbAI_Update_Player, "UPDATE_PLAYER");
    bbAI_Functions_add(self, AI_Command, bbAI_Command_NULL, "COMMAND_NULL");
    bbAI_Functions_add(self, AI_Command, bbAI_Command_Player, "COMMAND_PLAYER");
    return bbSuccess;
}
