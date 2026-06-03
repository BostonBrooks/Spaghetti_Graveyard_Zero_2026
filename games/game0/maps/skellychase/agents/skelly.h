
#include "core/core_inputs.h"

bbFlag bbAgent_Update_Skelly(bbAgent* agent)
{

    bbMoveable* agent_moveable= &home.agents_app.movables.moveables[agent->moveable];

    if (agent_moveable->type == bbMoveableType_Dead) return bbSuccess;

    bbMoveable* moveable = &home.agents_app.movables.moveables[0];
    I32 closest_int = 0;
    U32 distance = (agent_moveable->position.i - moveable->position.i)*
        (agent_moveable->position.i - moveable->position.i) +
            (agent_moveable->position.j - moveable->position.j)*
                (agent_moveable->position.j - moveable->position.j);


bbMoveable* closest_moveable = moveable;
    for (I32 i = 1; i < 8; i++)
    {
        moveable = &home.agents_app.movables.moveables[i];
        U32 new_distance = (agent_moveable->position.i - moveable->position.i)*
            (agent_moveable->position.i - moveable->position.i) +
                (agent_moveable->position.j - moveable->position.j)*
                    (agent_moveable->position.j - moveable->position.j);

        if (new_distance < distance)
        {
            closest_int = i;
            distance = new_distance;
            closest_moveable = moveable;
        }
    }

    if (distance > (36*POINTS_PER_TILE*POINTS_PER_TILE))
    {
        if (agent_moveable->type != bbMoveableType_Idle)
        {
            bbCoreInput_setMoveableIdle(&home.core.core, 69696969, agent->moveable,
                agent_moveable->position, bbInstructionSource_internal, no_handle);

            bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
            bbUI_Inbox_SetUnitState(&home.UI.inbox, entity->unit, bbDrawableState_idle);


        }

    }else if (closest_int!= agent_moveable->goal_moveable)
    {
        bbCoreInput_setGoalMoveable(&home.core.core, 69696969, agent->moveable,
            closest_int, bbInstructionSource_internal, no_handle);

        bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
        bbUI_Inbox_SetUnitState(&home.UI.inbox, entity->unit, bbDrawableState_moving);

        bbMapCoords position = agent_moveable->position;
        position.i += POINTS_PER_TILE;

        bbCoreInput_spawnAgent(&home.core.core, position,closest_moveable->position,1, bbInstructionSource_internal, no_handle);

    }

    return bbSuccess;
}



bbFlag bbAgent_Command_Skelly(bbAgent* agent,bbAgentCommandType type,bbAgentCommandData data)
{
    if (type == bbAC_damageAgent)
    {

        bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
        bbMoveable* moveable = &home.agents_app.movables.moveables[agent->moveable];

        if (moveable->type != bbMoveableType_Dead)
        {
            bbHandle agent_handle;
            bbVPool_reverseLookup(home.agents_app.agents->pool,agent,&agent_handle);
            //TODO use core input to set agent->health
            I32 HP = (agent->health - 100);
            bbUI_Inbox_SetUnitHP(&home.UI.inbox, entity->unit, (100.f*HP)/agent->max_health);
            bbCoreInput_damageAgent(&home.core.core, agent_handle, 100,bbInstructionSource_internal, no_handle);
        }
    }

    if (type == bbAC_killAgent)
    {
        bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
        bbMoveable* moveable = &home.agents_app.movables.moveables[agent->moveable];

        if (moveable->type != bbMoveableType_Dead)
        {
            bbAgentCommandData data;
            data.type = bbMoveableType_Dead;
            data.goal_point.i = 0;
            data.goal_point.j = 0;
            data.goal_point.k = 0;
            data.moveable = 0;
            bbCoreInput_setMoveableType(&home.core.core,0, agent->moveable, data,
                                             bbInstructionSource_internal,no_handle);

            bbUI_Inbox_SetUnitState(&home.UI.inbox, entity->unit, bbDrawableState_dead);
        }
    }

    return bbSuccess;
}