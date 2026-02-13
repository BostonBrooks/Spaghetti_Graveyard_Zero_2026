#ifndef BB_WIDGET_H
#define BB_WIDGET_H

#include "engine/userinterface/bbMouse.h"
#include "engine/userinterface/bbWidgetFunctions.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/graphics/bbCompositions.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/data/CSFML.h"
#include "engine/logic/bbTree.h"

typedef enum bbWidget_Type
{
    bbWidgetType_None,
    bbWidgetType_TextBox,
    bbWidgetType_SpellSelector,
    bbWidgetType_SpellSlot,
    bbWidgetType_CurrentSpell,
    bbWidgetType_Layer
} bbWidget_Type;

typedef enum bbWidget_state
{
    bbWidgetState_Default,
    bbWidgetState_Hover,
    bbWidgetState_Click,
    bbWidgetState_Frozen
} bbWidget_state;

typedef struct {
    I32 constructor;
    I32 update;
    I32 destructor;
    I32 command;
    I32 timer;
    I32 hide;
    I32 unhide;
} bbWidgetFunctionTable;

typedef struct bbWidget_TextBox
{
    I32 rows;
    I32 columns;
    char* string;
    sfText* text;
    sfFont* font;
}bbWidget_TextBox;

typedef union bbWidget_TypeData
{
    bbWidget_TextBox text_box;
} bbWidget_TypeData;

typedef struct bbWidget{
    //It is important that "bbTree_Node tree;" is the first element
    bbTree_Node tree;
    bbScreenPointsRect rect;
    bbMouseTable mtable;
    bbWidgetFunctionTable ftable;
    bbFrame frames[FRAMES_PER_WIDGET];
    bbWidget_Type type;
    bbWidget_state state;
    bool is_frozen;
    bbWidget_TypeData type_data;
    char key[KEY_LENGTH];



    void* extra_data;
} bbWidget;

typedef struct bbWidgets {
    bbVPool* pool;
    bbTree* tree;
    struct bbWidgetFunctions* functions;
    bbDictionary* dict;

    bbMouse* mouse;
    bbWidget* selected_textbox;
} bbWidgets;

bbFlag bbWidgets_init(bbWidgets* widgets);

bbFlag bbWidget_newEmpty(bbWidget** self,
                         bbWidgets* widgets,
                         bbWidget* parent,
                         char* name);

bbFlag bbWidget_constructor(bbWidget** self,
                             bbWidgets* widgets,
                             char* type,
                             char* parent,
                             char* name,
                             bbScreenPoints screen_points);

bbFlag bbWidget_draw(bbWidget* widget, drawFuncClosure* cl);

bbFlag bbWidgets_draw(bbWidgets* widgets, void* cl);

bbFlag bbWidget_onCommand(bbWidget* widget,
                          bbWidgets* widgets,
                          bbWidgetCommandType type,
                          bbHandle data);


bbFlag bbWidget_newLayout(bbWidget** self,
                         bbGraphicsApp* graphics,
                         bbWidgets* widgets,
                         bbWidget* parent);
#endif //BB_WIDGET_H