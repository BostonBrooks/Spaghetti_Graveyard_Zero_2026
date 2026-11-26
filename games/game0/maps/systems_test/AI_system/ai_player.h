

#include "player_send_goalpoint.h"
#include "core/actions.h"
#include "core/send_server_enitity.h"
#include "../core/action_request.h"
#include "engine/ECS/AI_system/bbAI_System.h"
#include "engine/ECS/graphics_system/bbGraphicsSystem_state.h"
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
                                         NULL);

                bbUI_Inbox_SetEntityState(&home.UI.inbox, entity_handle, bbDrawableState_idle);
                bbCI_Graphics_setState(&home.core.core,
                              NULL,
                              entity_handle,
                              bbDrawableState_idle,
                              home.core.core.simulation_time,
                                 bbInstructionSource_internal, no_handle);

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

                bbCI_Graphics_setState(&home.core.core,
                             NULL,
                             entity_handle,
                             bbDrawableState_attacking,
                             home.core.core.simulation_time,
                                bbInstructionSource_internal, no_handle);
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

    // bbHandle entity_handle;
    // bbECS_entity*  entity;
    // bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
    //                          bbECS_ECS, &entity_handle,
    //                          (bbComponent**)&entity);
    // bbDebug("command ai %s\n", entity->key);

    //is action: command is unable to modify state unless this function was called by a bbAction to ensure "core safeness"
    //I need to be careful about the player modifying the state without sending a message to the server and back
    if (type == bbAI_setGoalPoint && is_action)
    {
        bbHandle entity_handle;
        bbECS_entity*  entity;
        bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                                 bbECS_ECS, &entity_handle,
                                 (bbComponent**)&entity);
        //bbDebug("ai set goalpoint %s\n", entity->key);
        bbCI_Graphics_setState(&home.core.core,
                      NULL,
                      entity_handle,
                      bbDrawableState_moving,
                      home.core.core.simulation_time,
                         bbInstructionSource_internal, no_handle);

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
        bbHandle entity_handle;
        bbECS_entity*  entity;
        bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                                 bbECS_ECS, &entity_handle,
                                 (bbComponent**)&entity);

        bbHere()
        bbHandle target_handle = data.handle;

        bbHandle target_moveable_handle;
        bbMoveable* target_moveable;
        bbHandle_mapComponent(home.ECS.ECS,
                         bbECS_ECS,
                         target_handle,
                         bbECS_Moveables,
                         &target_moveable_handle,
                         (bbComponent**)&target_moveable);



        bbHandle attacker_moveable_handle;
        bbMoveable* attacker_moveable;
        bbComponent_mapComponent(home.ECS.ECS,
                         bbECS_AI,
                         (bbComponent*)component,
                         bbECS_Moveables,
                         &attacker_moveable_handle,
                         (bbComponent**)&attacker_moveable);

        bbECS_entity *arrow;
        bbCoreSynchronous_spawnEmptyEntity(&home.core.core,
                                           home.ECS.ECS,
                                           &arrow,
                                           "an arrow",
                                           bbInstructionSource_norewind,
                                           no_handle);

        bbHandle arrow_handle = arrow->component.entity_handle;

        bbMapCoords start_point = attacker_moveable->position;
        bbMapCoords end_point = target_moveable->position;


        I64 delta_i = end_point.i - start_point.i;
        I64 delta_j = end_point.j - start_point.j;
        I64 delta_k = end_point.k - start_point.k;
        I64 distance = bbArith64_sqrt2(delta_i*delta_i+delta_j*delta_j+delta_k*delta_k);
        I64 start_time = home.core.clock2_handle.map_tick;
        I64 end_time = 300 * distance / POINTS_PER_SQUARE + home.core.clock2_handle.map_tick;


        // bbGraphicsComponent_data data;

        //
        // bbUI_Inbox_NewArrow(&home.UI.inbox, &data);

        U64 deterministic_seed = bbArith64_hash(1234567);

        bbGraphicsComponent_data data3;
        data3.u.projectile.start_point = start_point;
        data3.u.projectile.end_point = end_point;
        data3.u.projectile.start_time = start_time;
        data3.u.projectile.end_time = end_time;
        data3.drawable_type = bbGraphicsType_projectile;
        data3.entity_handle = arrow_handle;
        data3.u.projectile.caster_entity_handle =entity_handle;
        bbCS_spawnGraphicsComponent(&home.core.core,
                                    NULL,
                                    "ARROW_CLOUD",
                                    &data3,
                                    bbInstructionSource_norewind,
                                    no_handle);

        bbProjectileMovement_data data2;

        data2.start_coords = start_point;
        data2.current_coords = start_point;
        data2.end_coords = end_point;
        data2.start_time = start_time;
        data2.end_time = end_time;
        data2.gravity = 1000;


        bbCS_spawnProjectileMovement(&home.core.core,
                                 arrow_handle,
                                 NULL,
                                 &data2,
                                 bbInstructionSource_norewind,
                                 no_handle);

        return bbSuccess;


    }


    //TODO type == bbAI_mapClick for testing purposes
    if (type == bbAI_clickMonster)
    {bbNotHere()
        bbAction action;
       // bbCoreInbox_TestClick3(&home.core.core, data.handle);
    }

    if (type == bbAI_mapClick)
    {bbNotHere()
/*
        if (data.integer == 0)
        {
            //TODO click to spawn
             bbCoreInput_testClick3(&home.core.core, data.goal_point,
                                    home.core.core.actual_time,
                                    bbInstructionSource_internal, no_handle);
        }
        else
        {//bbHere()
            // bbCoreInput_testClick4(&home.core.core, data.goal_point,
            //                        home.core.core.actual_time,
            //                        bbInstructionSource_internal, no_handle);
            bbHandle entity_handle;
            bbECS_entity* entity;
            bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                                     bbECS_ECS, &entity_handle,
                                     (bbComponent**)&entity);

            //bbDebug("send ai %s goalpoint\n", entity->key);

            bbCoreInput_sendAIGoalpoint(&home.core.core,
                                 entity_handle,
                                 data.goal_point,
                                 home.core.core.actual_time,
                                    bbInstructionSource_internal, no_handle);
        }
    */}
    return bbSuccess;
}
