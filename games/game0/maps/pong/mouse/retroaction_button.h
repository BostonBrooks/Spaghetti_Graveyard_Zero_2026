#include "engine/core/bbLocalMessageInputs.h"
#include "engine/data/bbHome.h"

bbFlag RetroactionButton_LeftDown (void* Mouse, void* Widgets, void* Widget, void*
                        Graphics)
{
    bbWidget* widget = (bbWidget*)Widget;
    bbGraphicsApp* graphics = Graphics;


    widget->is_frozen = true;

    bbLocalMessage_RetroactionUnfreeze(&home.core.core,"RETROACTION");
    return bbSuccess;
}
