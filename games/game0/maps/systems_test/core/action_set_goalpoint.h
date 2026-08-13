//bbFlag bbAction_fn(bbCore* core, bbAction* action);

#include "engine/core/bbCore.h"
#include "engine/ECS/ECS.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/moveables/bbMoveables.h"
#include "engine/ECS/moveables/bbMoveables_setState.h"


bbFlag bbAction_setGoalpoint_fn(bbCore* core, bbAction* action)
{
    bbHandle moveable_handle;
    bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS, action->handle,bbECS_Moveables, &moveable_handle, NULL);

    bbHandle action_handle;
    bbVPool_reverseLookup(core->action_pool,action,&action_handle);

    bbCI_Moveable_setGoalpoint(core,
                             moveable_handle,
                             action->map_coords,
                             bbInstructionSource_action,
                             action_handle);

    bbCore_react(core);

    return bbSuccess;
}

