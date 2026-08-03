#include "engine/core/bbCoreInboxInput.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"

bbFlag NetpauseButton_LeftDown (void* Mouse, void* Widgets, void* Widget, void*
                        Graphics)
{
    bbWidget* widget = (bbWidget*)Widget;
    bbGraphicsApp* graphics = Graphics;
    widget->is_frozen = true;
 //   bbCoreInbox_netpauseButton(&home.core.core, widget->key);
    return bbSuccess;
}
