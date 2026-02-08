#include "engine/userinterface/bbWidgets.h"

#include "engine/data/bbHome.h"
#include "engine/graphics/bbCompositions.h"
#include "engine/graphics/bbDrawfunctions.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbPrime.h"
#include "engine/logic/bbTree.h"
#include "engine/userinterface/bbWidgetFunctions.h"

bbFlag bbWidgets_init(bbWidgets* widgets){
    bbVPool_newBloated(&widgets->pool, sizeof(bbWidget), 1024, 1024);
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

            drawFunction(widget, frame, cl);

        }
    }
    return bbContinue;
}

bbFlag bbWidget_newEmpty(bbWidget** self, bbWidgets* widgets, bbWidget* parent, char* name)
{
    bbWidget* widget;
    bbHandle widget_handle;
    bbVPool_alloc(widgets->pool, (void**)&widget);
    bbVPool_reverseLookup(widgets->pool, widget, &widget_handle);

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

/* not yet implemented
    widget->mtable.isOver = -1;
    widget->mtable.Enter = -1;
    widget->mtable.Leave = -1;
    widget->mtable.LeftDown = -1;
    widget->mtable.LeftUp = -1;
    widget->mtable.LeftDrag = -1;
    widget->mtable.RightDown = -1;
    widget->mtable.RightUp = -1;
    widget->mtable.RightDrag = -1;
    widget->mtable.Drop = -1;
    widget->mtable.MouseIcon = -1;
    widget->mtable.DragIcon = -1;
    widget->mtable.hover = false;
    widget->mtable.selected = false;
*/
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
    bbHere()
    bbWidget* widget;

    bbHandle parent_handle;
    bbDictionary_lookup(widgets->dict, parent, &parent_handle);
    bbWidget* parent_widget;
    bbVPool_lookup(widgets->pool,(void**)&parent_widget,parent_handle);
    bbGraphicsApp* graphics = &home.UI.graphics;

    bbWidget_Constructor* function;
    bbFlag flag = bbWidgetFunctions_getFunction((void**)&function,widgets->functions,WidgetConstructor, type);
    if (flag == bbNone)
    {
        bbDebug("Constructor not found\n");
        return bbNone;
    }

    function(&widget, widgets, parent_widget, name, screen_points, graphics);

    if (self != NULL) *self = widget;
    return bbSuccess;
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

    *self = widget;

    return bbSuccess;
}