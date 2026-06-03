#include "core/core_inputs.h"
#include "engine/entities/bbAgentFunctions.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/core/bbLocalMessageInputs.h"
#include "engine/data/bbHome.h"
#include "engine/network/bbNetworkApp.h"

//typedef bbFlag bbAgent_Command (bbAgent* agent,
//                                   bbAgentCommandType type,
//                                   bbAgentCommandData data);
/*
bbFlag bbAgent_Command_Player(bbAgent* agent,bbAgentCommandType type,bbAgentCommandData data)
{
    if (type == bbAC_mapClick)
    {
            bbHandle handle = {0};
        if (data.movable == 0)
        {
            bbCoreInput_setGoalpointOut(&home.core.core,
                data.goal_point, home.core.clock2_handle.map_tick,bbInstructionSource_input,handle);
        } else
        {
            bbMovable* movable = &home.agents_app.movables.movables[agent->movable];

            //bbLocalMessage_SpawnUnit(&home.core.core, movable->position, data.goal_point, "BALLOON");

            bbCoreInput_spawnUnitOut(&home.core.core, 1,movable->position, data.goal_point, home.core.clock2_handle.map_tick
            ,bbInstructionSource_input,handle);
        }
    } else {
        bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
        bbUI_Inbox_SetUnitState(&home.UI.inbox, entity->unit, bbDrawableState_moving);
        bbCoreInput_setMovableType(&home.core.core,0, agent->movable, data,
                                     bbInstructionSource_internal,no_handle);
    }
    bbCore_react(&home.core.core);

    return bbSuccess;
}
*/
bbFlag bbAgent_Update_Fireball(bbAgent* agent)
{
    bbMovable* agent_movable= &home.agents_app.movables.movables[agent->movable];

    U32 distance = (agent_movable->position.i -agent_movable->goalpoint.i)*
    (agent_movable->position.i - agent_movable->goalpoint.i) +
        (agent_movable->position.j - agent_movable->goalpoint.j)*
            (agent_movable->position.j - agent_movable->goalpoint.j);




        if (distance < POINTS_PER_PIXEL*POINTS_PER_PIXEL)
    {
        //TODO only do this once
            if (agent_movable->type == bbMovableType_MovingThrough)
        {
            bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
            bbUI_Inbox_SetUnitState(&home.UI.inbox, entity->unit, bbDrawableState_attacking);

            bbAgentCommandData data;
            data.type = bbMovableType_Idle;
            data.goal_point = agent_movable->goalpoint;
            data.movable = 0;

            bbCoreInput_setMovableType(&home.core.core,0, agent->movable, data,
                                             bbInstructionSource_internal,no_handle);
        } else if (agent_movable->type == bbMovableType_Idle)
        {
            bbEntity* entity = &home.agents_app.entities.entity[agent->entity];
            bbMovable* movable = &home.agents_app.movables.movables[agent->movable];

            bbAgentCommandData data;
            data.type = bbMovableType_Unused;
            data.goal_point = agent_movable->goalpoint;
            data.movable = 0;
            bbCoreInput_setMovableType(&home.core.core,0, agent->movable, data,
                                       bbInstructionSource_internal,no_handle);


            //TODO undelete unit?
            bbUI_Inbox_DeleteUnit(&home.UI.inbox, agent->entity, agent->movable);

            bbCoreInput_DeleteEntity(&home.core.core,agent->entity,
                                       bbInstructionSource_internal,no_handle);

            for (I32 i = 0; i < NUM_ENTITIES; i++)
            {
                bbEntity* entity2 = &home.agents_app.entities.entity[i];
                bbAgent* agent2;

                bbFlag flag = bbVPool_lookup(home.agents_app.agents->pool,(void**)&agent2, entity2->agent);
                if (flag == bbFail) continue;
                if (entity2->movable.u64 == UINT64_MAX) continue;
                bbMovable* movable2 = &home.agents_app.movables.movables[entity2->movable.u64];


                U32 distance = (agent_movable->position.i -movable2->position.i)*
                               (agent_movable->position.i - movable2->position.i) +
                               (agent_movable->position.j - movable2->position.j)*
                               (agent_movable->position.j - movable2->position.j);



                if (distance < 4*POINTS_PER_TILE*POINTS_PER_TILE)
                {
                    bbAgentCommandData data;
                    //bbAgent2_onCommand(agent2,
                    //                  home.agents_app.agents,
                    //                   bbAC_damageAgent,
                    //                  data);
                }

            }


            //TODO  return bbDelete;?
        }


    }

    return bbSuccess;
}