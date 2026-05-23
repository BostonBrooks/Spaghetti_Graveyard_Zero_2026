
#include "engine/agents/bbAgentFunctions.h"
#include "engine/agents/bbMoveables.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "games/game0/maps/skellychase/agents/player.h"


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
    }

    return bbSuccess;
}


bbFlag bbAgentFunctions_populate(bbAgentFunctions* self)
{
    bbAgentFunctions_add(self, AgentUpdate, bbAgent_Update_Skelly,"UPDATE_SKELLY");
    bbAgentFunctions_add(self, AgentUpdate, bbAgent_Update_Player,"UPDATE_PLAYER");
    bbAgentFunctions_add(self, AgentCommand, bbAgent_Command_Player,"COMMAND_PLAYER");
    return bbSuccess;
}
