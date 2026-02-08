/* Game widget doesn't draw anything to the screen, it just acts as a layer/container for
 * The viewport, dialog, prompt and command textboxes */

#include "engine/data/bbHome.h"
#include "engine/data/CSFML.h"
#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/userinterface/bbWidgetFunctions.h"
#include "engine/logic/bbString.h"

bbFlag bbWidget_Constructor_Game (bbWidget** self,
                            bbWidgets* widgets,
                            bbWidget* parent,
                            char* name,
                            bbScreenPoints screen_coords,
                            bbGraphicsApp* graphics
                            )
{
    bbWidget* widget;
    bbWidget_newEmpty(&widget, widgets, parent, name);

    bbScreenPointsRect rect;


    rect.left = 0;
    rect.top = 0;
    rect.width = 720 * SCREEN_PPP;
    rect.height = 480 * SCREEN_PPP;
    widget->rect = rect;

    bbHandle dimensions;
    bbFlag flag;
    bbWidget* textbox_widget;

    flag = bbWidget_constructor(&textbox_widget,
                     &home.UI.widgets,
                     "TEXTBOX",
                     name,
                     "DIALOG",
                     (bbScreenPoints){497*SCREEN_PPP,12*SCREEN_PPP});

    dimensions.i32x2.x = 200*SCREEN_PPP;
    dimensions.i32x2.y = 322*SCREEN_PPP;
    if (flag == bbSuccess) bbWidget_onCommand(textbox_widget, widgets, bbWC_setDimensions, dimensions);


    flag = bbWidget_constructor(&textbox_widget,
                 &home.UI.widgets,
                 "TEXTBOX",
                 name,
                 "PROMPT",
                 (bbScreenPoints){497*SCREEN_PPP,355*SCREEN_PPP});



    dimensions.i32x2.x = 200*SCREEN_PPP;
    dimensions.i32x2.y = 45*SCREEN_PPP;
    if (flag == bbSuccess) bbWidget_onCommand(textbox_widget, widgets, bbWC_setDimensions, dimensions);

    flag = bbWidget_constructor(&textbox_widget,
                 &home.UI.widgets,
                 "TEXTBOX",
                 name,
                 "COMMAND",
                 (bbScreenPoints){497*SCREEN_PPP,416*SCREEN_PPP});


    dimensions.i32x2.x = 200*SCREEN_PPP;
    dimensions.i32x2.y = 45*SCREEN_PPP;
    if (flag == bbSuccess) bbWidget_onCommand(textbox_widget, widgets, bbWC_setDimensions, dimensions);
    dimensions.i32x2.x = 64*SCREEN_PPP;
    dimensions.i32x2.y = 1*SCREEN_PPP;
    if (flag == bbSuccess) bbWidget_onCommand(textbox_widget, widgets, bbWC_setBounds, dimensions);

    widgets->selected_textbox = textbox_widget;

    return bbSuccess;

}