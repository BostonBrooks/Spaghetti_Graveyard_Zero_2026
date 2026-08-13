//bbFlag bbAction_fn(bbCore* core, bbAction* action);

#include "engine/core/bbCore.h"
#include "engine/ECS/ECS.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/moveables/bbMoveables.h"


bbFlag bbAction_setGoalpoint_fn(bbCore* core, bbAction* action)
{
    bbHandle moveable_handle;
    bbHandle_mapComponent(home.ECS.ECS,bbECS_ECS, action->handle,bbECS_Moveables, &moveable_handle, NULL);

    bbMoveable_setGoalPoint((bbMoveables*)home.ECS.ECS->systems[bbECS_Moveables],
                            moveable_handle,
                            action->map_coords);

    return bbSuccess;
}

