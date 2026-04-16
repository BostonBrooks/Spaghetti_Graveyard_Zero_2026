#include "engine/core/bbLocalMessageInputs.h"
#include "engine/data/bbHome.h"

bbFlag Viewport_LeftDown (void* Mouse, void* Widgets, void* Widget, void*
                        Graphics)
{

    bbHere()
    bbMapCoords MC = {0,0,0};

    bbLocalMessage_SetViewpoint(&home.core.core, MC);
    return bbSuccess;
}
