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
        case bbUI_Inbox_newTux:
            bbUI_Inbox_newTux_fn(inbox, message);
            break;
        case bbUI_Inbox_setUnitSprite:
            bbHere()
            bbUI_Inbox_setUnitSprite_fn(inbox, message);
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
bbFlag bbUI_Inbox_newTux_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message)
{
    bbUnit* none;
    bbUnit_newTux(&none,home.viewport_app.units, &home.UI.graphics,
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

bbFlag bbUI_Inbox_NewTux(bbUI_Inbox* inbox, bbMapCoords coords, I32 moveable)
{
    bbUI_Inbox_message* message;
    bbThreadedQueue_alloc(&inbox->local_message_queue,(void**)&message);
    message->type = bbUI_Inbox_newTux;
    message->data.coords = coords;
    message->data.handle.handle.u64 = moveable;
    bbThreadedQueue_pushL(&inbox->local_message_queue, (void*)message);
    return bbSuccess;
}


bbFlag bbUI_Inbox_SetUnitSprite(bbUI_Inbox* inbox, bbHandle unit, I32 sprite)
{bbHere()
    bbUI_Inbox_message* message;
    bbThreadedQueue_alloc(&inbox->local_message_queue,(void**)&message);
    message->type = bbUI_Inbox_setUnitSprite;
    message->data.handle.handle = unit;
    message->data.integer = sprite;
    bbThreadedQueue_pushL(&inbox->local_message_queue, (void*)message);
    return bbSuccess;
}


bbFlag bbUI_Inbox_setUnitSprite_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message)
{
    bbUnit* unit;
    bbVPool_lookup(home.viewport_app.units->pool, (void**)&unit, message->data.handle.handle);
    bbHere()
    unit->drawable.state = message->data.integer;
    return bbSuccess;
}