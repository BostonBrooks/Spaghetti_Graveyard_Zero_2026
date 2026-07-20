#include "engine/core/bbCoreInboxInput.h"
#include "engine/data/bbHome.h"

bbFlag NetsendButton_LeftDown (void* Mouse, void* Widgets, void* Widget, void*
                        Graphics)
{
    bbWidget* widget = (bbWidget*)Widget;
    bbGraphicsApp* graphics = Graphics;


    widget->is_frozen = true;
    bbLocalMessage_NetsendButton(&home.core.core, widget->key);
    return bbSuccess;
}
