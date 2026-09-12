#include "engine/data/CSFML.h"
#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/userinterface/bbWidgetFunctions.h"
#include "engine/logic/bbString.h"
#include "engine/textbox/bbTextbox.h"

#include "widgets/bbWidget_textboxData.h"

bbFlag bbWidget_Constructor_Textbox(bbWidget** self,
                                    bbWidgets* widgets,
                                    bbWidget* parent,
                                    char* name,
                                    bbScreenPoints screen_points,
                                    bbGraphicsApp* graphics)
{
    bbHere()
    bbWidget* widget;

    bbWidget_newEmpty(&widget, widgets, parent, name);


    bbWidget_textboxData* extra_data = malloc(sizeof (bbWidget_textboxData));

#ifdef BB_CSFML3
    extra_data->sf_text = sfText_create(graphics->fonts->fonts[0]);
#else
    extra_data->sf_text = sfText_create();
    sfText_setFont(extra_data->sf_text, graphics->fonts->fonts[0]);
#endif

    sfText_setColor(extra_data->sf_text, sfBlack);
    sfText_setCharacterSize(extra_data->sf_text,12);
    extra_data->textbox = NULL;
    extra_data->text[0] = '\0';

    widget->extra_data = extra_data;

    bbScreenPointsRect rect;
    rect.left = screen_points.x;
    rect.top = screen_points.y;
    rect.width = 0 * SCREEN_PPP;
    rect.height = 0 * SCREEN_PPP;

    widget->rect = rect;
    bbHandle drawfunctionHandle;

    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                 "WIDGET_TEXTBOX",
                 &drawfunctionHandle);

    widget->frames[0].drawfunction = drawfunctionHandle.u64;
    widget->frames[0].offset.x = 3*SCREEN_PPP;
    widget->frames[0].offset.y = 3*SCREEN_PPP;


    if (self!=NULL) *self = widget;

    return bbSuccess;
}

