#include "engine/agents/bbAgentFunctions.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/network/bbNetworkApp.h"

//typedef bbFlag bbAgent_Command (bbAgent* agent,
//                                   bbAgentCommandType type,
//                                   bbAgentCommandData data);

bbFlag bbAgent_Command_Player(bbAgent* agent,bbAgentCommandType type,bbAgentCommandData data)
{
    bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
    bbUI_Inbox_SetUnitState(&home.UI.inbox, entity->unit, bbDrawableState_moving);
    bbCoreInput_setMoveableType(&home.core.core,0, agent->moveable, data,
                                     bbInstructionSource_internal,no_handle);
    bbCore_react(&home.core.core);

    return bbSuccess;
}

bbFlag bbAgent_Update_Player(bbAgent* agent)
{
    bbMoveable* agent_movable= &home.agents_app.movables.moveables[agent->moveable];

    U32 distance = (agent_movable->position.i -agent_movable->goalpoint.i)*
    (agent_movable->position.i - agent_movable->goalpoint.i) +
        (agent_movable->position.j - agent_movable->goalpoint.j)*
            (agent_movable->position.j - agent_movable->goalpoint.j);


    if (agent_movable->type != bbMoveableType_Idle)
    {
        //TODO only do this once
        if (distance < POINTS_PER_PIXEL*POINTS_PER_PIXEL)
        {
            bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
            bbUI_Inbox_SetUnitState(&home.UI.inbox, entity->unit, bbDrawableState_idle);

            bbAgentCommandData data;
            data.type = bbMoveableType_Idle;
            data.goal_point = agent_movable->goalpoint;
            data.moveable = 0;

            bbCoreInput_setMoveableType(&home.core.core,0, agent->moveable, data,
                                             bbInstructionSource_internal,no_handle);
        }
    }
    return bbSuccess;
}