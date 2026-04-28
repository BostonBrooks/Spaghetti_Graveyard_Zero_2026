#include "engine/core/bbLocalMessageInputs.h"
#include "engine/data/bbHome.h"

bbFlag SwitchCharacterButton_LeftDown (void* Mouse, void* Widgets, void* Widget, void*
                        Graphics)
{
    bbWidget* widget = (bbWidget*)Widget;
    bbGraphicsApp* graphics = Graphics;

    bbLocalMessage_SwitchCharacterButton(&home.core.core, widget->key);
    return bbSuccess;
}
