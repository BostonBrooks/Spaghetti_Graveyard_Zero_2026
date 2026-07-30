#include "engine/userinterface/bbWidgets.h"

#include "engine/data/bbHome.h"
#include "engine/graphics/bbCompositions.h"
#include "engine/graphics/bbDrawfunctions.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbPrime.h"
#include "engine/logic/bbString.h"
#include "engine/logic/bbTree.h"
#include "engine/userinterface/bbWidgetFunctions.h"

bbFlag bbWidgets_init(bbWidgets* widgets){
    bbVPool_newBloated(&widgets->pool, sizeof(bbWidget), 1024, 1024, "bbWidget");
    bbTree_new(&widgets->tree, widgets->pool, offsetof(bbWidget, tree));

    bbDictionary_new(&widgets->dict, nextPrime(256));
    bbWidgetFunctions_new(&widgets->functions);

    bbWidgetFunctions_populate(widgets->functions);

    return  bbSuccess;
}

bbFlag bbWidget_drawFunc(bbTree* tree, void* node, void* cl){
    bbWidget* widget = node;
    return bbWidget_draw(widget, cl);

}

bbFlag bbWidget_draw(bbWidget* widget, drawFuncClosure* cl){
    for (I32 i = 0; i < FRAMES_PER_WIDGET; i++){
        bbFrame* frame = &widget->frames[i];

        bbGraphicsApp* graphics = cl->graphics;
        if (frame->drawfunction >= 0 && frame->drawfunction < graphics->drawfunctions->num) {

            bbDrawFunction *drawFunction =
                    graphics->drawfunctions->functions[frame->drawfunction];
            if (drawFunction == NULL) return bbContinue;
            drawFunction(widget, frame, cl);

        }
    }
    return bbContinue;
}

bbFlag bbWidget_newEmpty(bbWidget** self, bbWidgets* widgets, bbWidget* parent, char* name)
{
    bbWidget* widget;
    bbHandle widget_handle;
    bbVPool_alloc2(widgets->pool, (void**)&widget,&widget_handle);

    bbStr_putStr(widget->key, name, KEY_LENGTH);

    widget->tree.visible = true;
    widget->tree.children_visible = true;

    if (parent == NULL)
    {
        widgets->tree->root = widget_handle;
    } else
    {
        bbTreeNode_setParent(widgets->tree, widget, parent);
    }

    widget->rect.left = 0;
    widget->rect.top = 0;
    widget->rect.width = 0;
    widget->rect.height = 0;


    widget->mtable.is_over = -1;
    widget->mtable.enter = -1;
    widget->mtable.leave = -1;
    widget->mtable.left_down = -1;
    widget->mtable.left_up = -1;
    widget->mtable.left_drag = -1;
    widget->mtable.right_down = -1;
    widget->mtable.right_up = -1;
    widget->mtable.right_drag = -1;
    widget->mtable.drop = -1;
    widget->mtable.mouse_icon = -1;
    widget->mtable.drag_icon = 154;
    widget->mtable.hover = false;
    widget->mtable.selected = false;

    widget->ftable.constructor = -1;
    widget->ftable.update = -1;
    widget->ftable.destructor = -1;
    widget->ftable.command = -1;
    widget->ftable.timer = -1;
    widget->ftable.hide = -1;
    widget->ftable.unhide = -1;

    for (I32 i = 0; i < FRAMES_PER_WIDGET; i++) {
        widget->frames[i].drawfunction = -1;
        widget->frames[i].handle.u64 = 0;
        widget->frames[i].start_time = 0;
        widget->frames[i].offset.x = 0;
        widget->frames[i].offset.y = 0;
        widget->frames[i].framerate = 1.f;
        widget->frames[i].type = Sprite;
    }

    widget->angle = 0;
    widget->type = bbWidgetType_None;
    widget->state = bbWidgetState_Default;
    widget->is_frozen = false;

    bbDictionary_add(widgets->dict, name, widget_handle);

    *self = widget;
    return bbSuccess;

}

bbFlag bbWidgets_draw(bbWidgets* widgets, void* cl) {

    bbTree* tree = widgets->tree;
    bbVPool* pool = tree->pool;
    void* root;
    bbVPool_lookup(pool, &root, tree->root);
    return bbTree_descendingMapVisible(tree, root, bbWidget_drawFunc, cl);
}


bbFlag bbWidget_constructor(bbWidget** self,
                             bbWidgets* widgets,
                             char* type,
                             char* parent,
                             char* name,

                             bbScreenPoints screen_points)
{
    bbWidget* widget;

    bbHandle parent_handle;
    bbDictionary_lookup(widgets->dict, parent, &parent_handle);
    bbWidget* parent_widget;
    bbVPool_lookup(widgets->pool,(void**)&parent_widget,parent_handle);
    bbGraphicsApp* graphics = &home.UI.graphics;

    bbWidget_Constructor* function;
    bbFlag flag = bbWidgetFunctions_getFunction((void**)&function,widgets->functions,WidgetConstructor, type);
    if (flag == bbNone || function == NULL)
    {
        bbDebug("Constructor not found\n");
        return bbNone;
    }

    function(&widget, widgets, parent_widget, name, screen_points, graphics);

    if (self != NULL) *self = widget;
    return bbSuccess;
}


bbFlag bbWidget_onCommand(bbWidget* widget,
                          bbWidgets* widgets,
                          bbWidgetCommandType type,
                          bbHandle data)
{
    I32 func_int = widget->ftable.command;
    if (func_int < 0) return bbNone;
    bbFlag (*funcPtr)(bbWidget*, bbWidgetCommandType, bbHandle);
    funcPtr = widgets->functions->commands[func_int];
    return funcPtr(widget, type, data);
}

bbFlag bbWidget_newLayout(bbWidget** self,
                          bbGraphicsApp* graphics,
                          bbWidgets* widgets,
                          bbWidget* parent){

    bbWidget* widget;

    bbWidget_newEmpty(&widget, widgets, parent,"LAYOUT");

    bbScreenPointsRect rect;
    rect.left = 0;
    rect.top = 0;
    rect.width = 720 * SCREEN_PPP;
    rect.height = 480 * SCREEN_PPP;

    widget->rect = rect;

    int funcInt;
    funcInt = bbMouseFunctions_getInt(&widgets->mouse->functions,MouseIsOver,
                                  "HOVER");
    widget->mtable.is_over = funcInt;

    widget->mtable.mouse_icon = 154;

    bbHandle drawfunctionHandle;
    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                     "WIDGET_SPRITE",
                     &drawfunctionHandle);

    widget->frames[0].drawfunction = drawfunctionHandle.u64;

    bbDictionary_lookup(graphics->sprites->dictionary,
                        "LAYOUT_480", &widget->frames[0].handle);

    //bbDebug("LAYOUT_480 = %d\n", widget->frames[0].handle.u64);
    widget->frames[0].offset.x = 0;
    widget->frames[0].offset.y = 0;

    bbHandle handle;
    bbVPool_reverseLookup(widgets->pool, widget, &handle);
    bbDictionary_add(widgets->dict, "LAYOUT", handle);

    if (self!=NULL) *self = widget;

    return bbSuccess;
}
#ifdef DEFINE_PONG
bbFlag bbWidget_newLayoutPong(bbWidget** self,
                          bbGraphicsApp* graphics,
                          bbWidgets* widgets,
                          bbWidget* parent){

    bbWidget* widget;

    bbWidget_newEmpty(&widget, widgets, parent,"LAYOUT");

    bbScreenPointsRect rect;
    rect.left = 0;
    rect.top = 0;
    rect.width = 720 * SCREEN_PPP;
    rect.height = 480 * SCREEN_PPP;

    widget->rect = rect;

    int funcInt;
    funcInt = bbMouseFunctions_getInt(&widgets->mouse->functions,MouseIsOver,
                                  "HOVER");
    widget->mtable.is_over = funcInt;

    widget->mtable.mouse_icon = 85;

    bbHandle drawfunctionHandle;
    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                     "WIDGET_SPRITE",
                     &drawfunctionHandle);

    widget->frames[0].drawfunction = drawfunctionHandle.u64;

    bbDictionary_lookup(graphics->sprites->dictionary,
                        "PONGLAYOUT", &widget->frames[0].handle);

    //bbDebug("LAYOUT_480 = %d\n", widget->frames[0].handle.u64);
    widget->frames[0].offset.x = 0;
    widget->frames[0].offset.y = 0;

    bbHandle handle;
    bbVPool_reverseLookup(widgets->pool, widget, &handle);
    bbDictionary_add(widgets->dict, "LAYOUT", handle);

    if (self!=NULL) *self = widget;

    return bbSuccess;
}




#endif //DEFINE_PONG

bbFlag bbWidget_newViewport(bbWidget** self, bbGraphicsApp* graphics,
                            bbWidgets* widgets, bbWidget* parent,
                            void* viewport){

    bbWidget* widget;
    bbWidget_newEmpty(&widget, widgets, parent, "VIEWPORT");
    widget->extra_data = viewport;

    bbViewport* viewport1 = viewport;
    viewport1->widget = widget;


    bbScreenPointsRect rect;
    rect.left = 12 * POINTS_PER_PIXEL;
    rect.top = 12 * POINTS_PER_PIXEL;
    rect.width = 466 * POINTS_PER_PIXEL;
    rect.height = 456 * POINTS_PER_PIXEL;


    int funcInt = bbMouseFunctions_getInt(&widgets->mouse->functions,
                                          MouseIsOver,
                                      "VIEWPORT");
    widget->mtable.is_over = funcInt;

    funcInt = bbMouseFunctions_getInt(&widgets->mouse->functions,
                                          MouseLeftDown,
                                      "VIEWPORT");
    widget->mtable.left_down = funcInt;

    funcInt = bbMouseFunctions_getInt(&widgets->mouse->functions,
                                          MouseRightDown,
                                      "VIEWPORT");
    widget->mtable.right_down = funcInt;

    widget->mtable.mouse_icon = 85;
    widget->mtable.drag_icon = -1;

    widget->mtable.enter = -1;
    widget->mtable.leave = -1;
    widget->mtable.left_up = -1;
    widget->mtable.left_drag = -1;

    widget->rect = rect;

    bbHandle drawfunctionHandle;

    bbDictionary_lookup(graphics->drawfunctions->dictionary, "WIDGET_VIEWPORT",
                        &drawfunctionHandle);

    widget->frames[0].drawfunction = drawfunctionHandle.u64;

    for (I32 i = 1; i < FRAMES_PER_WIDGET; i++) {
        widget->frames[i].drawfunction = -1;
    }

    bbHandle handle;
    bbVPool_reverseLookup(widgets->pool, widget, &handle);
    bbDictionary_add(widgets->dict, "VIEWPORT", handle);


    *self = widget;

    return bbSuccess;

}