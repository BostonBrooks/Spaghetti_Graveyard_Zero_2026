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
    //bbHere()
}
