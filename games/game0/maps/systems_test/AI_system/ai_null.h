#include "engine/data/bbHome.h"
#include "engine/ECS/spawn_entity.h"
#include "engine/ECS/AI_system/bbAI_System.h"
#include "engine/ECS/moveables/bbMoveables_setState.h"
#include "engine/logic/bbTerminal.h"

bbFlag bbAI_Update_NULL(bbAI_Component* component)
{
    //bbHere()
}

/// must change state with core safe methods. do not change state if !is_action or be carefull and test often
bbFlag bbAI_Command_NULL(bbAI_Component* component,
                         bbAI_CommandType type,
                         bbAI_CommandData data,
                         bool is_action)
{
    if (type == bbAI_setGoalPoint && is_action)
    {


        bbHandle moveable_handle;

        bbComponent_mapComponent(home.ECS.ECS,
            bbECS_AI,
            (bbComponent*)component,
            bbECS_Moveables,
            &moveable_handle,
            NULL);


        bbCS_Moveable_setGoalpoint(&home.core.core,
                                   moveable_handle,
                                   data.goal_point,
                                   bbInstructionSource_internal,
                                   no_handle);
    }

    if (type == bbAI_mapClick)
    {
        bbCoreInput_testClick3(&home.core.core, data.goal_point, home.core.core.actual_time, bbInstructionSource_internal, no_handle);
    }
    return bbSuccess;
}
