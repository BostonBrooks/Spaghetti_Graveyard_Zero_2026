
#include "core/core_inputs.h"

bbFlag bbAgent_Update_Skelly(bbAgent* agent)
{

    bbMovable* agent_movable= &home.agents_app.movables.movables[agent->movable];

    if (agent_movable->type == bbMovableType_Dead) return bbSuccess;

    bbMovable* movable = &home.agents_app.movables.movables[0];
    I32 closest_int = 0;
    U32 distance = (agent_movable->position.i - movable->position.i)*
        (agent_movable->position.i - movable->position.i) +
            (agent_movable->position.j - movable->position.j)*
                (agent_movable->position.j - movable->position.j);


bbMovable* closest_movable = movable;
    for (I32 i = 1; i < 8; i++)
    {
        movable = &home.agents_app.movables.movables[i];
        U32 new_distance = (agent_movable->position.i - movable->position.i)*
            (agent_movable->position.i - movable->position.i) +
                (agent_movable->position.j - movable->position.j)*
                    (agent_movable->position.j - movable->position.j);

        if (new_distance < distance)
        {
            closest_int = i;
            distance = new_distance;
            closest_movable = movable;
        }
    }

    if (distance > (36*POINTS_PER_TILE*POINTS_PER_TILE))
    {
        if (agent_movable->type != bbMovableType_Idle)
        {
            bbCoreInput_setMovableIdle(&home.core.core, 69696969, agent->movable,
                agent_movable->position, bbInstructionSource_internal, no_handle);

            bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
            bbUI_Inbox_SetUnitState(&home.UI.inbox, entity->unit, bbDrawableState_idle);


        }

    }else if (closest_int!= agent_movable->goal_movable)
    {
        bbCoreInput_setGoalMovable(&home.core.core, 69696969, agent->movable,
            closest_int, bbInstructionSource_internal, no_handle);

        bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
        bbUI_Inbox_SetUnitState(&home.UI.inbox, entity->unit, bbDrawableState_moving);

        bbMapCoords position = agent_movable->position;
        position.i += POINTS_PER_TILE;

        bbCoreInput_spawnAgent(&home.core.core, position,closest_movable->position,1, bbInstructionSource_internal, no_handle);

    }

    return bbSuccess;
}



bbFlag bbAgent_Command_Skelly(bbAgent* agent,bbAgentCommandType type,bbAgentCommandData data)
{
    if (type == bbAC_damageAgent)
    {

        bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
        bbMovable* movable = &home.agents_app.movables.movables[agent->movable];

        if (movable->type != bbMovableType_Dead)
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
        bbMovable* movable = &home.agents_app.movables.movables[agent->movable];

        if (movable->type != bbMovableType_Dead)
        {
            bbAgentCommandData data;
            data.type = bbMovableType_Dead;
            data.goal_point.i = 0;
            data.goal_point.j = 0;
            data.goal_point.k = 0;
            data.movable = 0;
            bbCoreInput_setMovableType(&home.core.core,0, agent->movable, data,
                                             bbInstructionSource_internal,no_handle);

            bbUI_Inbox_SetUnitState(&home.UI.inbox, entity->unit, bbDrawableState_dead);
        }
    }

    return bbSuccess;
}