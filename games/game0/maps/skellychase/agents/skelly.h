
#include "core/core_inputs.h"

bbFlag bbAgent_Update_Skelly(bbAgent* agent)
{

    bbMoveable* agent_movable= &home.agents_app.movables.moveables[agent->moveable];
    bbMoveable* moveable = &home.agents_app.movables.moveables[0];
    I32 closest_moveable = 0;
    U32 distance = (agent_movable->position.i - moveable->position.i)*
        (agent_movable->position.i - moveable->position.i) +
            (agent_movable->position.j - moveable->position.j)*
                (agent_movable->position.j - moveable->position.j);



    for (I32 i = 1; i < 8; i++)
    {
        moveable = &home.agents_app.movables.moveables[i];
        U32 new_distance = (agent_movable->position.i - moveable->position.i)*
            (agent_movable->position.i - moveable->position.i) +
                (agent_movable->position.j - moveable->position.j)*
                    (agent_movable->position.j - moveable->position.j);

        if (new_distance < distance)
        {
            closest_moveable = i;
            distance = new_distance;
        }
    }

    if (distance > (36*POINTS_PER_TILE*POINTS_PER_TILE))
    {
        if (agent_movable->type != bbMoveableType_Idle)
        {
            bbCoreInput_setMoveableIdle(&home.core.core, 69696969, agent->moveable,
                agent_movable->position, bbInstructionSource_internal, no_handle);

            bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
            bbUI_Inbox_SetUnitState(&home.UI.inbox, entity->unit, bbDrawableState_idle);
        }

    }else if (closest_moveable!= agent_movable->goal_moveable)
    {
        bbCoreInput_setGoalMoveable(&home.core.core, 69696969, agent->moveable,
            closest_moveable, bbInstructionSource_internal, no_handle);

        bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
        bbUI_Inbox_SetUnitState(&home.UI.inbox, entity->unit, bbDrawableState_moving);

        bbAgentCommandData data;
        bbAgent2_onCommand(agent,
                          home.agents_app.agents,
                           bbAC_damageAgent,
                          data);
    }

    return bbSuccess;
}

bbFlag bbAgent_Command_Skelly(bbAgent* agent,bbAgentCommandType type,bbAgentCommandData data)
{
    if (type == bbAC_damageAgent)
    {

        bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
        bbUI_Inbox_SetUnitState(&home.UI.inbox, entity->unit, bbDrawableState_moving);

        bbHandle agent_handle;
        bbVPool_reverseLookup(home.agents_app.agents->pool,agent,&agent_handle);
        //TODO use core input to set agent->health
        I32 HP = (agent->health - 100);
        bbUI_Inbox_SetUnitHP(&home.UI.inbox, entity->unit, (100.f*HP)/agent->max_health);
        bbCoreInput_damageAgent(&home.core.core, agent_handle, 100,bbInstructionSource_internal, no_handle);

    }

    return bbSuccess;
}