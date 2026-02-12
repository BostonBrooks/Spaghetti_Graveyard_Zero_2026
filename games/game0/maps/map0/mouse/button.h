#include "engine/core/bbLocalMessageInputs.h"
#include "engine/data/bbHome.h"

bbFlag Button_LeftDown (void* Mouse, void* Widgets, void* Widget, void*
                        Graphics)
{
    bbWidget* widget = (bbWidget*)Widget;
    bbGraphicsApp* graphics = Graphics;


    bbDictionary_lookup(graphics->sprites->dictionary,
                        "BUTTON_CLICK", &widget->frames[0].handle);

    bbLocalMessage_PrintString(&home.core.core, "You clicked a button\n");
    return bbSuccess;
}
