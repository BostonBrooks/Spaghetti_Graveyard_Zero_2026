#include "core/core_inputs.h"
#include "engine/entities/bbAgentFunctions.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/core/bbLocalMessageInputs.h"
#include "engine/data/bbHome.h"
#include "engine/network/bbNetworkApp.h"

//typedef bbFlag bbAgent_Command (bbAgent* agent,
//                                   bbAgentCommandType type,
//                                   bbAgentCommandData data);

bbFlag bbAgent_Command_Player(bbAgent* agent,bbAgentCommandType type,bbAgentCommandData data)
{
    if (type == bbAC_mapClick)
    {
            bbHandle handle = {0};
        if (data.moveable == 0)
        {
            bbCoreInput_setGoalpointOut(&home.core.core, agent->entity,
                data.goal_point, home.core.clock2_handle.map_tick,bbInstructionSource_input,handle);
        } else
        {
            bbMoveable* moveable = &home.agents_app.movables.moveables[agent->moveable];

            //bbLocalMessage_SpawnUnit(&home.core.core, moveable->position, data.goal_point, "BALLOON");

            bbCoreInput_spawnUnitOut(&home.core.core, 1,moveable->position, data.goal_point, home.core.clock2_handle.map_tick
            ,bbInstructionSource_input,handle);
            bbCore_react(&home.core.core);
        }
    } else if (type == bbAC_setGoalPoint) {
        bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
        bbUI_Inbox_SetUnitState(&home.UI.inbox, entity->unit, bbDrawableState_moving);
        bbCoreInput_setMoveableType(&home.core.core,0, agent->moveable, data,
                                     bbInstructionSource_internal,no_handle);
    }


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