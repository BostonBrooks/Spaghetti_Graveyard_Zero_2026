
#include "engine/userinterface/bbTextInput.h"

bbFlag bbWidget_Constructor_TextInput(bbWidget** self,
                                      bbWidgets* widgets,
                                      bbWidget* parent,
                                      char* name,
                                      bbScreenPoints screen_points,
                                      bbGraphicsApp* graphics)
{
    bbHere()
    bbWidget* widget;

    bbWidget_newEmpty(&widget, widgets, parent, name);


    bbTextInput* extra_data;
    bbTextInput_new(&extra_data, widget);

    bbTextInput_setStr(extra_data, "test 123\n");
#ifdef BB_CSFML3
    extra_data->sf_text = sfText_create(graphics->fonts->fonts[0]);
    sfText_setFillColor(extra_data->sf_text, sfBlack);
#else
    extra_data->sf_text = sfText_create();
    sfText_setFont(extra_data->sf_text, graphics->fonts->fonts[0]);
    sfText_setColor(extra_data->sf_text, sfBlack);
#endif

    sfText_setCharacterSize(extra_data->sf_text,12);

    widget->extra_data = extra_data;

    bbHandle command_handle;
    bbDictionary_lookup(widgets->functions->command_dict,"WIDGET_TEXT_INPUT",&command_handle);
    widget->ftable.command = command_handle.u64;

    bbScreenPointsRect rect;
    rect.left = screen_points.x;
    rect.top = screen_points.y;
    rect.width = 0 * SCREEN_PPP;
    rect.height = 0 * SCREEN_PPP;

    widget->rect = rect;
    bbHandle drawfunctionHandle;

    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                 "WIDGET_TEXT_INPUT",
                 &drawfunctionHandle);

    widget->frames[0].draw_function = drawfunctionHandle.u64;
    widget->frames[0].offset.x = 3*SCREEN_PPP;
    widget->frames[0].offset.y = 3*SCREEN_PPP;


    if (self!=NULL) *self = widget;

    return bbSuccess;
}

bbFlag bbWidget_Command_TextInput (bbWidget* widget,
                                   bbWidgetCommandType type,
                                   bbHandle data)
{
    bbTextInput* text_input = widget->extra_data;

    switch (type)
    {
    case bbWC_putChar:
        {
            bbTextInput_putChar(text_input, data.u64);
            break;
        }
    case bbWC_putStr:
        {
            bbTextInput_putStr(text_input, data.ptr);
            break;
        }
    case bbWC_setBounds:
        {

            bbTextInput_setBounds(text_input, data.i32x2.x, data.i32x2.y);
            break;
        }
    case bbWC_setDimensions:
        {
            break;
        }
    case bbWC_setStr:
        {
        bbTextInput_setStr(text_input, data.ptr);
            break;
        }
    case bbWC_clrStr:
        {
        bbTextInput_setStr(text_input, "");
            break;
        }
    case bbWC_setPosition:
        {

            widget->rect.left = data.i32x2.x;
            widget->rect.top = data.i32x2.y;
            break;
        }
    default:
        bbNotHere()
    }
}