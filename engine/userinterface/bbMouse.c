
#include "engine/userinterface/bbMouse.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/userinterface/bbMouseFunctions.h"

bbFlag bbMouse_Init(bbMouse* mouse, void* Widgets, bbGraphicsApp* graphics)
{
    bbWidgets* widgets = (bbWidgets*)Widgets;
    mouse->position.x = 720*4;
    mouse->position.y = 480*4;
    mouse->left_down = false;
    mouse->right_down = false;
    mouse->left_changed = false;
    mouse->right_changed = false;
    mouse->was_over = widgets->pool->null;
    mouse->is_over = widgets->pool->null;
    mouse->selected = widgets->pool->null;

    widgets->mouse = mouse;
    return bbSuccess;
}

bbFlag bbMouse_Event(bbMouse* mouse, sfEvent* event)
{
    sfMouseButton button;
    switch (event->type){
        case sfEvtMouseMoved:
            mouse->position = pixel_getScreenPoints(event->mouseMove.x,
                                                event->mouseMove.y);

        break;
        case sfEvtMouseButtonPressed:
            button = event->mouseButton.button;
            mouse->position = pixel_getScreenPoints(event->mouseButton.x,event->mouseButton.y);
            if (button == sfMouseLeft)
            {
                mouse->left_changed = true;
                mouse->left_down = true;
                mouse->previous_position = mouse->position;

            } else if (button == sfMouseRight)
            {
                mouse->right_changed = true;
                mouse->right_down = true;
            }
            break;
        case sfEvtMouseButtonReleased:

            button = event->mouseButton.button;
            mouse->position = pixel_getScreenPoints(event->mouseButton.x,event->mouseButton.y);
            if (button == sfMouseLeft)
            {
                mouse->left_changed = true;
                mouse->left_down = false;
            } else if (button == sfMouseRight)
            {
                mouse->right_changed = true;
                mouse->right_down = false;
            }
            break;
    default:{
            bbDebug("input not recognised\n");
    }
    }
    return bbSuccess;
}

bbFlag bbMouse_isOver(bbMouse* mouse, void* Widgets)
{
    bbWidgets* widgets = (bbWidgets*)Widgets;

    bbMouse_isOver_cl cl;
    cl.widgets = widgets;

    bbTree* tree = widgets->tree;
    bbVPool* pool = tree->pool;
    void* root;
    bbVPool_lookup(pool, &root, tree->root);
    return bbTree_ascendingMapVisible(tree, root, bbMouse_isOverFunc, &cl);
    return bbSuccess;
}

bbFlag bbMouse_Update(bbMouse* mouse, void* Widgets, bbGraphicsApp* graphics)
{

    bbWidgets* widgets = Widgets;
    bbVPool* pool = widgets->pool;
    if(!bbVPool_handleIsEqual(pool, mouse->was_over, pool->null)){
        bbWidget* toLeave;
        bbWidget* toEnter;

        bbVPool_lookup(pool, (void**)&toLeave, mouse->was_over);
        bbVPool_lookup(pool, (void**)&toEnter, mouse->is_over);
        bbMouse_LeaveWidget(mouse, widgets, toLeave, graphics);
        bbMouse_EnterWidget(mouse, widgets, toEnter, graphics);

        mouse->was_over = pool->null;

    }
    if (mouse->left_down && mouse->left_changed){

        bbWidget* widget;
        bbVPool_lookup(pool, (void**)&widget, mouse->is_over);
        bbMouse_LeftDownWidget (mouse, widgets, widget, graphics);

    } else if (!mouse->left_down && mouse->left_changed) {
        bbWidget* widget;
        if(!bbVPool_handleIsEqual(pool, mouse->selected, pool->null)) {
            bbVPool_lookup(pool, (void **) &widget, mouse->selected);
            bbMouse_LeftUpWidget(mouse, widgets, widget, graphics);
        }
    } else if (mouse->left_down && !mouse->left_changed) {
        bbWidget* widget;
        if(!bbVPool_handleIsEqual(pool, mouse->selected, pool->null)) {
            bbVPool_lookup(pool, (void **) &widget, mouse->selected);
            bbMouse_LeftDragWidget(mouse, widgets, widget, graphics);
        }
    }


    return bbSuccess;
}



bbFlag bbMouse_isOverFunc(bbTree* tree, void* node, void* cl)
{
    bbWidget* widget = (bbWidget*)node;
    bbMouse_isOver_cl* closure = (bbMouse_isOver_cl*)cl;
    bbWidgets* widgets = closure->widgets;
    bbMouse* mouse = widgets->mouse;

    I32 funcInt = widget->mtable.is_over;

    if (funcInt == -1) return bbContinue;

    bbMouse_IsOver* func = mouse->functions.IsOver[funcInt];

    if (func == NULL) return bbContinue;
    return func(mouse, widgets, widget);

}

bbFlag bbMouse_Draw(bbMouse* mouse, void* Widgets, bbGraphicsApp* graphics,
                    sfRenderWindow* window)
{

    bbWidgets* widgets = (bbWidgets*)Widgets;

    bbHandle selected_handle = mouse->selected;
    if (!bbVPool_handleIsEqual(widgets->pool, selected_handle,
                              widgets->pool->null)){
        bbWidget* selected;
        bbVPool_lookup(widgets->pool, (void**)&selected, selected_handle);

        if (selected->mtable.drag_icon >= 0){
            bbScreenPoints dragged_SP;
            dragged_SP.x = mouse->position.x - mouse->drag_origin.x;
            dragged_SP.y = mouse->position.y - mouse->drag_origin.y;
            sfVector2f pos = bbScreenPoints_getV2f(dragged_SP);
            sfSprite* sprite = graphics->sprites->sprites[selected->mtable.drag_icon];
            sfSprite_setPosition(sprite, pos);
            sfRenderWindow_drawSprite(window, sprite, NULL);


        }

    }

    sfVector2f pos;
    bbHandle widget_handle = mouse->is_over;
    bbWidget* widget;
    bbVPool_lookup(widgets->pool, (void**)&widget, widget_handle);

    int spriteInt;
    pos = bbScreenPoints_getV2f(mouse->position);

    spriteInt = (widget == NULL) ? 133 : widget->mtable.mouse_icon;

    if (spriteInt < 0) spriteInt = 0;
    sfSprite* sprite = graphics->sprites->sprites[spriteInt];
    sfSprite_setPosition(sprite, pos);
    sfRenderWindow_drawSprite(window, sprite, NULL);
    return bbSuccess;
}

bbFlag bbMouse_EnterWidget(void* mouse, void* widgets, void* Widget, void*
graphics)
{
    bbWidget* widget = Widget;
    bbMouse* Mouse = mouse;
    I32 funcInt = widget->mtable.enter;

    bbDebug("you entered widget %s\n", widget->key);
    if (funcInt == -1) return bbSuccess;

    bbMouse_Leave* func = Mouse->functions.Enter[funcInt];

    return func(mouse, widgets, Widget, graphics);
}

bbFlag bbMouse_LeaveWidget(void* Mouse, void* widgets, void* Widget, void*
graphics)
{
    bbWidget* widget = Widget;
    bbMouse* mouse = Mouse;
    I32 funcInt = widget->mtable.leave;

    bbDebug("you left widget %s\n", widget->key);
    if (funcInt == -1) return bbSuccess;

    bbMouse_Leave* func = mouse->functions.Leave[funcInt];

    return func(Mouse, widgets, Widget, graphics);
}

bbFlag bbMouse_LeftDownWidget(void* Mouse, void* widgets, void* Widget,
                                void* graphics)
{
    bbWidget* widget = Widget;
    bbMouse* mouse = Mouse;
    I32 funcInt = widget->mtable.left_down;

    bbDebug("you clicked widget %s\n", widget->key);

    if (funcInt == -1) return bbSuccess;

    bbMouse_Leave* func = mouse->functions.LeftDown[funcInt];

    return func(Mouse, widgets, Widget, graphics);
}
bbFlag bbMouse_LeftUpWidget(void* Mouse, void* widgets, void* Widget,
                                  void* graphics)
{
    bbWidget* widget = Widget;
    bbMouse* mouse = Mouse;
    I32 funcInt = widget->mtable.left_up;

    bbDebug("you unclicked widget %s\n", widget->key);
    if (funcInt == -1) return bbSuccess;

    bbMouse_Leave* func = mouse->functions.LeftUp[funcInt];

    return func(Mouse, widgets, Widget, graphics);
}

bbFlag bbMouse_LeftDragWidget(void* Mouse, void* widgets, void* Widget,
                            void* graphics)
{
    bbWidget* widget = Widget;
    bbMouse* mouse = Mouse;
    I32 funcInt = widget->mtable.left_drag;

    if (funcInt == -1) return bbSuccess;

    bbMouse_Leave* func = mouse->functions.LeftDrag[funcInt];

    return func(Mouse, widgets, Widget, graphics);
}