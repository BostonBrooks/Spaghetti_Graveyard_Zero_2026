#include "engine/userinterface/bbUI_Inbox.h"

#include "engine/userinterface/bbWidgets.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbString.h"
#include "engine/logic/bbTerminal.h"

#define UI_INBOX_SIZE 4097

bbFlag bbUI_Inbox_init(bbUI_Inbox* inbox)
{
    bbVPool_newThreaded(&inbox->local_message_pool,
                 sizeof(bbUI_Inbox_message),
                 UI_INBOX_SIZE);

    bbThreadedQueue_init(&inbox->local_message_queue,
                      inbox->local_message_pool,
                      sizeof(bbUI_Inbox_message),
                      UI_INBOX_SIZE,offsetof(bbUI_Inbox_message, list_element));

    return bbSuccess;
}
bbFlag bbUI_Inbox_setWidgetPosition_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message);

bbFlag bbUI_Inbox_check(bbUI_Inbox* inbox)
{
    bbUI_Inbox_message* message;

    bbFlag flag;

    while (1)
    {
        flag = bbThreadedQueue_popR(&inbox->local_message_queue,(void**)&message);
        if (flag != bbSuccess) return bbSuccess;

        switch (message->type)
        {
        case bbUI_Inbox_unpressButton:
            bbUI_Inbox_unpressButton_fn(inbox, message);
            break;

        case bbUI_Inbox_unpressButton2:
            bbUI_Inbox_unpressButton2_fn(inbox, message);
            break;

        case bbUI_Inbox_setWidgetPosition:
            bbUI_Inbox_setWidgetPosition_fn(inbox, message);
            break;

        case bbUI_Inbox_setViewpoint:
            bbUI_Inbox_setViewpoint_fn(inbox, message);
            break;
#ifdef DEFINE_SKELLYCHASE
        case bbUI_Inbox_newSkelly:
            bbUI_Inbox_newSkelly_fn(inbox, message);
            break;
        case bbUI_Inbox_setUnitState:
            bbHere()
            bbUI_Inbox_setUnitState_fn(inbox, message);
            break;
        case bbUI_Inbox_newBanana:
            bbUI_Inbox_newBanana_fn(inbox, message);
            break;
#endif
        default:

            bbDebug("Unknown UI local message type\nmessage->type = %d\n", message->type);

        }
            bbThreadedQueue_free(&inbox->local_message_queue, (void**)&message);
    }
    return bbNone;
}

bbFlag bbUI_Inbox_unpressButton_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message)
{
    bbWidgets* widgets = &home.UI.widgets;
    bbHandle widget_handle;
    bbWidget* widget;

    bbDictionary_lookup(widgets->dict,"REACT",&widget_handle);
    bbVPool_lookup(widgets->pool,(void**)&widget,widget_handle);

    widget->is_frozen = false;

    //bbDebug("Button unpressed\n");
    return bbSuccess;
}

bbFlag bbUI_Inbox_unpressButton2_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message)
{
    bbWidgets* widgets = &home.UI.widgets;
    bbHandle widget_handle;
    bbWidget* widget;

    bbDictionary_lookup(widgets->dict,message->data.string.string,&widget_handle);
    bbVPool_lookup(widgets->pool,(void**)&widget,widget_handle);

    widget->is_frozen = false;

    return bbSuccess;
}

bbFlag bbUI_Inbox_setWidgetPosition_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message)
{
    bbWidgets* widgets = &home.UI.widgets;
    bbHandle widget_handle;
    bbWidget* widget;

    bbDictionary_lookup(widgets->dict,message->data.string.string,&widget_handle);
    bbVPool_lookup(widgets->pool,(void**)&widget,widget_handle);

    //bbDebug("str = %s\n", message->data.string.string);
    bbWidget_onCommand(widget, widgets,bbWC_setPosition,message->data.handle.handle);

    return bbSuccess;
}

bbFlag bbUI_Inbox_setViewpoint_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message)
{
    home.viewport_app.viewport.viewpoint = message->data.coords;

    return bbSuccess;
}

#ifdef DEFINE_SKELLYCHASE
bbFlag bbUI_Inbox_newSkelly_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message)
{
    bbUnit* none;
    bbUnit_newSkelly(&none,home.viewport_app.units, &home.UI.graphics,
    message->data.coords, message->data.handle.handle.u64);
    return bbSuccess;
}
#endif

bbFlag bbUI_Inbox_UnpressButton(bbUI_Inbox* inbox)
{
    bbUI_Inbox_message* message;
    bbThreadedQueue_alloc(&inbox->local_message_queue,(void**)&message);
    message->type = bbUI_Inbox_unpressButton;
    bbThreadedQueue_pushL(&inbox->local_message_queue, (void*)message);
    return bbSuccess;
}

bbFlag bbUI_Inbox_UnpressButton2(bbUI_Inbox* inbox, char* key)
{
    bbUI_Inbox_message* message;
    bbThreadedQueue_alloc(&inbox->local_message_queue,(void**)&message);
    message->type = bbUI_Inbox_unpressButton2;
    bbStr_setStr(message->data.string.string, key, KEY_LENGTH);

    //bbDebug("key: %s\n", key);

    bbThreadedQueue_pushL(&inbox->local_message_queue, (void*)message);
    return bbSuccess;
}


bbFlag bbUI_Inbox_SetWidgetPosition(bbUI_Inbox* inbox, char* key, bbHandle position)
{
    bbUI_Inbox_message* message;
    bbThreadedQueue_alloc(&inbox->local_message_queue,(void**)&message);
    message->type = bbUI_Inbox_setWidgetPosition;
    bbStr_setStr(message->data.string.string, key, KEY_LENGTH);
    message->data.handle.handle = position;
    bbThreadedQueue_pushL(&inbox->local_message_queue, (void*)message);
    return bbSuccess;
}


bbFlag bbUI_Inbox_SetViewpoint(bbUI_Inbox* inbox, bbMapCoords MC)
{
    bbUI_Inbox_message* message;
    bbThreadedQueue_alloc(&inbox->local_message_queue,(void**)&message);
    message->type = bbUI_Inbox_setViewpoint;
    message->data.coords = MC;
    bbThreadedQueue_pushL(&inbox->local_message_queue, (void*)message);
    return bbSuccess;
}

bbFlag bbUI_Inbox_NewSkelly(bbUI_Inbox* inbox, bbMapCoords coords, I32 moveable)
{
    bbUI_Inbox_message* message;
    bbThreadedQueue_alloc(&inbox->local_message_queue,(void**)&message);
    message->type = bbUI_Inbox_newSkelly;
    message->data.coords = coords;
    message->data.handle.handle.u64 = moveable;
    bbThreadedQueue_pushL(&inbox->local_message_queue, (void*)message);
    return bbSuccess;
}


bbFlag bbUI_Inbox_SetUnitState(bbUI_Inbox* inbox, bbHandle unit, I32 sprite)
{bbHere()
    bbUI_Inbox_message* message;
    bbThreadedQueue_alloc(&inbox->local_message_queue,(void**)&message);
    message->type = bbUI_Inbox_setUnitState;
    message->data.handle.handle = unit;
    message->data.integer = sprite;
    bbThreadedQueue_pushL(&inbox->local_message_queue, (void*)message);
    return bbSuccess;
}


bbFlag bbUI_Inbox_setUnitState_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message)
{
    bbUnit* unit;
    bbVPool_lookup(home.viewport_app.units->pool, (void**)&unit, message->data.handle.handle);
    bbHere()
    unit->drawable.state = message->data.integer;
    return bbSuccess;
}


bbFlag bbUI_Inbox_NewBanana(bbUI_Inbox* inbox, bbMapCoords MC, I32 entity_index)
{
    bbUI_Inbox_message* message;
    bbThreadedQueue_alloc(&inbox->local_message_queue,(void**)&message);
    message->type = bbUI_Inbox_newBanana;
    message->data.coords = MC;
    message->data.integer = entity_index;
    bbThreadedQueue_pushL(&inbox->local_message_queue, (void*)message);
    return bbSuccess;
}


bbFlag bbUI_Inbox_newBanana_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message)
{

    bbViewportApp* app = &home.viewport_app;
    bbMapCoords MC = message->data.coords;
    I32 index = message->data.integer;
    //char key[KEY_LENGTH];
    //sscanf(string, "%[^','],%d,%d,%d", key, &MC.i, &MC.j,&index);



    bbDebug("spawn banana i = %d, j = %d, k = %d\n", MC.i, MC.j, MC.k);


    bbUnit* unit;


        bbUnits* units = home.viewport_app.units;
        bbGraphicsApp* graphics = &home.UI.graphics;
        bbVPool* pool = units->pool;
        bbSquareCoords SC = bbMapCoords_getSquareCoords(MC);
        bbUnitSquare* unitSquare = bbDrawables_getSquare(units,SC.i, SC.j, units->squares_i, units->squares_j);

        bbFlag flag = bbVPool_alloc(pool, (void**)&unit);

        bbHandle unit_handle;
        bbVPool_reverseLookup(pool, unit, &unit_handle);

        unit->drawable.coords = MC;
        bbHandle drawfunctionHandle;

        unit->prev_coords = MC;
        unit->prev_time = 0;
        unit->prev_goalpoint = MC;
        unit->next_coords = MC;
        unit->next_time = 2;
        unit->next_goalpoint = MC;
        unit->drawable.state = 0;

        bbDictionary_lookup(graphics->drawfunctions->dictionary,
                            "UNIT_SPRITE",
                            &drawfunctionHandle);

        unit->drawable.frames[0].drawfunction = drawfunctionHandle.u64;
        unit->drawable.frames[0].handle.u64 = 627;
        unit->drawable.frames[0].start_time=  0;
        unit->drawable.frames[0].framerate = 1;
        unit->drawable.frames[0].offset.x = 0;
        unit->drawable.frames[0].offset.y = 0;

        bbDictionary_lookup(graphics->drawfunctions->dictionary,
                        "DRAWABLE_SHADOW",
                        &drawfunctionHandle);

        unit->drawable.frames[1].drawfunction = drawfunctionHandle.u64;
        unit->drawable.frames[1].handle.u64 = 612;
        unit->drawable.frames[1].start_time = 0;
        unit->drawable.frames[1].framerate = 0;
        unit->drawable.frames[1].offset.x = 0;
        unit->drawable.frames[1].offset.y = 0;


    bbDictionary_lookup(graphics->drawfunctions->dictionary,
                    "MAPICON_TEST",
                    &drawfunctionHandle);

    unit->drawable.frames[2].drawfunction = drawfunctionHandle.u64;
    unit->drawable.frames[2].handle.u64 = 628;
    unit->drawable.frames[2].start_time = 0;
    unit->drawable.frames[2].framerate = 0;
    unit->drawable.frames[2].offset.x = 0;
    unit->drawable.frames[2].offset.y = 0;

        for (I32 k = 3; k < FRAMES_PER_DRAWABLE; k++){
            unit->drawable.frames[k].drawfunction = -1;
        }

        home.viewport_app.unit_array[index] = unit_handle;

        bbList_sortL(&unitSquare->list, unit);

        unit->enitity = index;
        home.entities.entity[unit->enitity].unit = unit_handle;

    return bbSuccess;

}