
#include "engine/data/bbHome.h"
#include "engine/ECS/AI_system/bbAI_System.h"
#include "engine/ECS/moveables/bbMoveables_setState.h"
#include "engine/logic/bbTerminal.h"

bbFlag bbAI_Update_NULL(bbAI_Component* component)
{
    bbHere()
}

bbFlag bbAI_Command_NULL(bbAI_Component* component,
                                   bbAI_CommandType type,
                                   bbAI_CommandData data)
{
    bbDebug("goalpoint = %d, %d\n", data.goal_point.i, data.goal_point.j);

    bbHandle moveable_handle;

    bbComponent_mapComponent(home.ECS.ECS,bbECS_AI, (bbComponent*)component,bbECS_Moveables,&moveable_handle,NULL);


    bbCS_Moveable_setGoalpoint(&home.core.core,
                         moveable_handle,
                         data.goal_point,
                         bbInstructionSource_internal,
                         no_handle);

    return bbSuccess;
}