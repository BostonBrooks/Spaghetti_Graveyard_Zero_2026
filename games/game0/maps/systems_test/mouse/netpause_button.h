#include "engine/core/bbCoreInboxInput.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "core/core_inbox.h"
#include "games/game0/maps/systems_test/core/netpause_button.h"

bbFlag NetpauseButton_LeftDown (void* Mouse, void* Widgets, void* Widget, void*
                                Graphics)
{
    bbWidget* widget = (bbWidget*)Widget;
    bbGraphicsApp* graphics = Graphics;
    widget->is_frozen = true;

    //bbCoreInbox_TestMessage(&home.core.core);
    bbCoreInbox_NetpauseButton(&home.core.core, widget->key);
    return bbSuccess;
}
