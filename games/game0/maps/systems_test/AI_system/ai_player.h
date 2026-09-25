#include "player_send_goalpoint.h"
#include "core/actions.h"
#include "core/send_server_enitity.h"
#include "../core/action_request.h"
#include "engine/ECS/AI_system/bbAI_System.h"
#include "engine/logic/bbString.h"

extern U32 collision;

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

        break;
        }

    case bbAIState_Approaching:
        {
            bbMoveable* moveable;
            bbHandle moveable_handle;
            bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                                     bbECS_Moveables, &moveable_handle,
                                     (bbComponent**)&moveable);

            I64 delta_i = moveable->goalpoint.i - moveable->position.i;
            I64 delta_j = moveable->goalpoint.j - moveable->position.j;

            I64 distance_squared = delta_i * delta_i + delta_j * delta_j;

            if (distance_squared<=POINTS_PER_TILE * POINTS_PER_TILE)
            {
                bbHandle entity_handle;

                bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                                         bbECS_ECS, &entity_handle,
                                         NULL);

                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_attacking);
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

            break;
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


        bbCI_Moveable_setGoalpoint(&home.core.core,
                                   moveable_handle,
                                   data.goal_point,
                                   bbInstructionSource_internal,
                                   no_handle);



    }

    if (type == bbAI_targetMonster && is_action)
    {

        bbHandle target_handle = data.handle;


        bbHandle target_moveable_handle;
        bbMoveable* target_moveable;
        bbHandle_mapComponent(home.ECS.ECS,
                         bbECS_ECS,
                         target_handle,
                         bbECS_Moveables,
                         &target_moveable_handle,
                         (bbComponent**)&target_moveable);


        bbHandle entity_handle;
        bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                                 bbECS_ECS, &entity_handle,
                                 NULL);

        bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_moving);

        bbHandle AI_handle;
        bbComponent_getHandle(&home.ECS.AI_system.system,(bbComponent*)component,&AI_handle);

        bbCI_AI_setApproaching(&home.core.core,
             AI_handle,
             target_handle,
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


        bbCI_Moveable_setGoalMovable(&home.core.core,
                                   moveable_handle,
                                   target_moveable_handle,
                                   bbInstructionSource_internal,
                                   no_handle);



    }


    //TODO type == bbAI_mapClick for testing purposes
    if (type == bbAI_clickMonster)
    {
        bbAction action;
        bbCoreInbox_TestClick3(&home.core.core, data.handle);
    }

    if (type == bbAI_mapClick)
    {

        if (data.integer == 0)
        {
            //TODO click to spawn
             bbCoreInput_testClick3(&home.core.core, data.goal_point,
                                    home.core.core.actual_time,
                                    bbInstructionSource_internal, no_handle);
        }
        else
        {bbHere()
            // bbCoreInput_testClick4(&home.core.core, data.goal_point,
            //                        home.core.core.actual_time,
            //                        bbInstructionSource_internal, no_handle);
            bbHandle entity_handle;
            bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                                     bbECS_ECS, &entity_handle,
                                     NULL);
            bbCoreInput_sendAIGoalpoint(&home.core.core,
                                 entity_handle,
                                 data.goal_point,
                                 home.core.core.actual_time,
                                    bbInstructionSource_internal, no_handle);
        }
    }
    return bbSuccess;
}
