#include "engine/ECS/AI_system/bbAI_System.h"

bbFlag bbAI_Update_Striking(bbAI_Component* component)
{
    if (component->state == bbAIState_Recovering) return bbSuccess; //unit dead

    bbHitPoints hitpoints;
    bbHitPoint* hitpoint;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_Hitpoints, NULL,
                             (bbComponent**)&hitpoint);
    bbHandle attacker_entity_handle;
    bbECS_entity* attacker_entity;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_ECS, &attacker_entity_handle,
                             (bbComponent**)&attacker_entity);

    bbMoveable* moveable;
    bbHandle moveable_handle;
    bbComponent_mapComponent(home.ECS.ECS, bbECS_AI, (bbComponent*)component,
                             bbECS_Moveables, &moveable_handle,
                             (bbComponent**)&moveable);

    bbHandle AI_handle;
    bbComponent_getHandle(home.ECS.ECS->systems[bbECS_AI],(bbComponent*)component,&AI_handle);
    if (hitpoint->current_health <=0)
    {//set unit dead

        bbUI_Inbox_SetEntityState(&home.UI.inbox, attacker_entity_handle, bbDrawableState_dead);
        bbCI_Moveable_setDead(&home.core.core, moveable_handle,
                         bbInstructionSource_internal, no_handle);

        bbHandle AI_handle;
        bbComponent_getHandle(home.ECS.ECS->systems[bbECS_AI],(bbComponent*)component,&AI_handle);

        bbCI_AI_setRecovering(&home.core.core,
                       AI_handle, home.core.core.simulation_time,
                 bbInstructionSource_internal, no_handle);

        return bbSuccess;
    }

    if (component->state == bbAIState_Idle)
    {
STATE_IDLE:        //find target

        bbMoveable* target_moveable;
        bbHandle target_moveable_handle;


        bbPlayers* players = &home.ECS.players;
        bbHandle target_entity_handle = home.ECS.ECS->system.pool->null;
        bbFlag flag = bbSpatial_findNearestTarget(&home.core.core, home.ECS.ECS, attacker_entity_handle , &target_entity_handle, POINTS_PER_SQUARE);


        if (bbSuccess != bbVPool_handleIsNULL(home.ECS.ECS->system.pool,target_entity_handle)) {
            //Cant find target
            bbFlag_print(flag)
            return bbSuccess;
        }


        bbUI_Inbox_SetEntityState(&home.UI.inbox, attacker_entity_handle, bbDrawableState_moving);

        bbHandle goal_moveable_handle;
        bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS,target_entity_handle,bbECS_Moveables,&goal_moveable_handle,NULL);

        bbCI_Moveable_setGoalMovable(&home.core.core, moveable_handle,
                     goal_moveable_handle,
                     bbInstructionSource_internal, no_handle);
        bbHandle AI_handle;
        bbComponent_getHandle(home.ECS.ECS->systems[bbECS_AI],(bbComponent*)component,&AI_handle);

        bbCI_AI_setApproaching(&home.core.core,
                       AI_handle,target_entity_handle,
                       home.core.core.simulation_time,
                 bbInstructionSource_internal, no_handle);

    } else if (component->state == bbAIState_Approaching)
    {
        //get within range

        bbHandle target_entity_handle = component->target;
        bbMoveable* target_moveable;

        bbHandle_mapComponent(home.ECS.ECS,
                              bbECS_ECS,
                              target_entity_handle,
                              bbECS_Moveables,
                              NULL,
                              (bbComponent**)&target_moveable);


        U64 distance_squared = (moveable->position.i - target_moveable->position.i)
                             * (moveable->position.i - target_moveable->position.i)
                             + (moveable->position.j - target_moveable->position.j)
                             * (moveable->position.j - target_moveable->position.j);

        if (distance_squared <POINTS_PER_TILE * 4 * POINTS_PER_TILE * 4)
        {
            bbUI_Inbox_SetEntityState(&home.UI.inbox, attacker_entity_handle, bbDrawableState_attacking);

            bbCI_Moveable_setIdle(&home.core.core,
                             moveable_handle,
                             bbInstructionSource_internal, no_handle);



            bbCI_AI_setStriking(&home.core.core,
                                   AI_handle,
                                   target_entity_handle,
                                   home.core.core.simulation_time,
                             bbInstructionSource_internal, no_handle);

            component->last_attack = home.core.core.simulation_time;
        }

    } else if (component->state == bbAIState_Striking)
    {

        //if out of range, set state idle, go to first option



            bbHandle target_entity_handle = component->target;
            bbMoveable* target_moveable;


        bbAI_Component* target_ai;
        bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS,target_entity_handle,bbECS_AI,NULL,(bbComponent**)&target_ai);

        if (home.core.core.simulation_time > component->last_attack+ 60)
        {
            component->last_attack = home.core.core.simulation_time;
            bbCI_Hitpoints_damage(&home.core.core,
                                  home.ECS.ECS,
                                  target_entity_handle,
                                  0,
                                  50,
                                  bbInstructionSource_internal, no_handle);
        }


            bbHandle_mapComponent(home.ECS.ECS,
                                  bbECS_ECS,
                                  target_entity_handle,
                                  bbECS_Moveables,
                                  NULL,
                                  (bbComponent**)&target_moveable);


            U64 distance_squared = (moveable->position.i - target_moveable->position.i)
                                 * (moveable->position.i - target_moveable->position.i)
                                 + (moveable->position.j - target_moveable->position.j)
                                 * (moveable->position.j - target_moveable->position.j);

        if (distance_squared >POINTS_PER_TILE * 5 * POINTS_PER_TILE * 5 || target_ai->state == bbAIState_Recovering )
        {
            bbUI_Inbox_SetEntityState(&home.UI.inbox, attacker_entity_handle, bbDrawableState_idle);

            bbCI_Moveable_setIdle(&home.core.core,
                             moveable_handle,
                             bbInstructionSource_internal, no_handle);


            bbCI_AI_setIdle(&home.core.core,
                                   AI_handle,
                                   home.core.core.simulation_time,
                             bbInstructionSource_internal, no_handle);

            goto STATE_IDLE;
        }

    }
    return bbSuccess;

}
