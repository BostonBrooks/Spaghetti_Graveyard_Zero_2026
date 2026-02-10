//textbox is a textbox

#include "engine/data/CSFML.h"
#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/userinterface/bbWidgetFunctions.h"
#include "engine/logic/bbString.h"

bbFlag bbWidget_Constructor_Textbox (bbWidget** self,
                                  bbWidgets* widgets,
                                  bbWidget* parent,
                                  char* name,
                                  bbScreenPoints screen_points,
                                  bbGraphicsApp* graphics)
{
    bbWidget* widget;
    bbFlag flag = bbWidget_newEmpty(&widget, widgets, parent, name);
    bbAssert(widget != NULL, "null address\n");


    bbScreenPointsRect rect;

    rect.left = screen_points.x;
    rect.top = screen_points.y;
    rect.width = 50*POINTS_PER_PIXEL;
    rect.height = 11*POINTS_PER_PIXEL;

    widget->rect = rect;

    widget->ftable.command = bbWidgetFunctions_getInt(widgets->functions,WidgetCommand ,"TEXTBOX");

    int funcInt;
    funcInt = bbMouseFunctions_getInt(&widgets->mouse->functions,MouseIsOver,
                                  "HOVER");
    widget->mtable.is_over = funcInt;
/*
    funcInt = bbMouseFunctions_getInt(&widgets->mouse->functions,MouseLeftDown,
                                  "TEXTBOX");
    widget->mtable.LeftDown = funcInt;
*/
    widget->mtable.mouse_icon = 155;

    bbGraphicsApp* Graphics = graphics;
    bbHandle drawfunctionHandle;
    bbDictionary_lookup(Graphics->drawfunctions->dictionary, "TEXTBOX",
                        &drawfunctionHandle);
    widget->frames[0].drawfunction = drawfunctionHandle.u64;
    widget->frames[0].offset.x = 5;
    widget->frames[0].offset.y = 5;

    bbDictionary_lookup(Graphics->drawfunctions->dictionary, "TEXTBOX_INDICATOR",
                        &drawfunctionHandle);
    widget->frames[1].drawfunction = drawfunctionHandle.u64;

    widget->type =  bbWidgetType_TextBox;

    char* string = calloc(1048, sizeof(char));

    widget->type_data.text_box.rows = 25;
    widget->type_data.text_box.columns = 25;

    bbStr_setStr(string , "this is a textbox", 1048);
    bbStr_setBounds(string , widget->type_data.text_box.columns, widget->type_data.text_box.rows, 1048);
    widget->type_data.text_box.string = string;


    sfText* text = sfText_create();
    sfText_setString(text, widget->type_data.text_box.string);
    sfText_setFont(text, Graphics->fonts->fonts[0]);
    bbScreenPoints pts;
    pts.x = screen_points.x + widget->frames[0].offset.x;
    pts.y = screen_points.y + widget->frames[0].offset.y;
    sfVector2f pos;
    pos = bbScreenPoints_getV2f(pts);
    sfText_setPosition(text, pos);
    sfText_setCharacterSize(text, 15);
    sfText_setColor(text, sfBlack);

    widget->type_data.text_box.text = text;


    if (self != NULL) *self = widget;

    return bbSuccess;
}

//typedef bbFlag bbWidget_OnCommand (bbWidget* widget,bbWidgetCommandType type,  void* data);
bbFlag Textbox_OnCommand(bbWidget* widget, bbWidgetCommandType type, bbHandle data){
    switch(type)
    {
         case bbWC_putChar:
             {
                 char ch = data.u64;
                 char* string = widget->type_data.text_box.string;
                 bbStr_putChar(string, ch, 1048);
                 bbStr_setBounds(string , widget->type_data.text_box.columns,
                                 widget->type_data.text_box.rows, 1048);

                 sfText_setString(widget->type_data.text_box.text, string);


                 break;
             }

    case bbWC_putStr:
             {
                 char* ch = data.ptr;
                 char* string = widget->type_data.text_box.string;
                 bbStr_putStr(string, ch, 1048);
                 bbStr_setBounds(string , widget->type_data.text_box.columns,
                               widget->type_data.text_box.rows, 1048);

                 sfText_setString(widget->type_data.text_box.text, string);


                 break;
             }
        //TODO set textbox size in points. Bounds will be calculated based on font, font size and area
        case bbWC_setBounds:
             {
                 I32x2 bounds = data.i32x2;

                 widget->type_data.text_box.columns = bounds.x;
                 widget->type_data.text_box.rows = bounds.y;
                 bbStr_setBounds(widget->type_data.text_box.string,
                     widget->type_data.text_box.columns,
                     widget->type_data.text_box.rows,
                     1048);

                 sfText_setString(widget->type_data.text_box.text, widget->type_data.text_box.string);
                 break;
             }

    case bbWC_setDimensions:
             { //TODO get letter spacing and line spacing
                 I32x2 dimensions = data.i32x2;

                 widget->rect.width = dimensions.x;
                 widget->rect.height = dimensions.y;

                 sfText* text = widget->type_data.text_box.text;
                 float lineSpacing = 12.f;// != sfText_getLineSpacing(text);
                 float letterSpacing = 8.f;// != sfText_getLetterSpacing(text);
                 //int charsize = sfText_getCharacterSize(text);

                 //bbDebug("lineSpacing = %f, letterSpacing = %f, charsize = %d\n", lineSpacing, letterSpacing, charsize);


                 I32x2 bounds;
                 bounds.x = dimensions.x / (letterSpacing * SCREEN_PPP);
                 bounds.y = dimensions.y / (lineSpacing * SCREEN_PPP);

                 widget->type_data.text_box.columns = bounds.x;
                 widget->type_data.text_box.rows = bounds.y;
                 bbStr_setBounds(widget->type_data.text_box.string,
                     widget->type_data.text_box.columns,
                     widget->type_data.text_box.rows,
                     1048);

                 sfText_setString(widget->type_data.text_box.text,
                     widget->type_data.text_box.string);
                 break;
             }
    case bbWC_setStr:
             {
                 char* ch = data.ptr;
                 char* string = widget->type_data.text_box.string;
                 bbStr_setStr(string, ch, 1048);
                 bbStr_setBounds(string , widget->type_data.text_box.columns,
                     widget->type_data.text_box.rows, 1048);

                 sfText_setString(widget->type_data.text_box.text, string);


                 break;
             }
        case bbWC_clrStr:
        {
            char* ch = data.ptr;
            char* string = widget->type_data.text_box.string;
            bbStr_setStr(string, "", 1048);
            bbStr_setBounds(string , widget->type_data.text_box.columns,
                widget->type_data.text_box.rows, 1048);

            sfText_setString(widget->type_data.text_box.text, string);


            break;
        }

    }
    return bbSuccess;
}
