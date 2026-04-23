#include "engine/core/bbLocalMessageInputs.h"
#include "engine/data/bbHome.h"

bbFlag PauseButton_LeftDown (void* Mouse, void* Widgets, void* Widget, void*
                        Graphics)
{
    bbWidget* widget = (bbWidget*)Widget;
    bbGraphicsApp* graphics = Graphics;


    widget->is_frozen = !widget->is_frozen;



    bbClock_testPause(&home.clock2,widget->is_frozen);


    return bbSuccess;
}
