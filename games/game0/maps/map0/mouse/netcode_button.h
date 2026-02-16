#include "engine/core/bbLocalMessageInputs.h"
#include "engine/data/bbHome.h"

bbFlag NetcodeButton_LeftDown (void* Mouse, void* Widgets, void* Widget, void*
                        Graphics)
{
    bbWidget* widget = (bbWidget*)Widget;
    bbGraphicsApp* graphics = Graphics;


    widget->is_frozen = true;
    bbLocalMessage_NetcodeButton(&home.core.core, widget->key,home.UI.UI_time + 60);
    return bbSuccess;
}
