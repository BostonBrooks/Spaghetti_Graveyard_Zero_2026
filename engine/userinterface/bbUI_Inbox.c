#include "engine/userinterface/bbUI_Inbox.h"

#include "engine/userinterface/bbWidgets.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbTerminal.h"

bbFlag bbUI_Inbox_init(bbUI_Inbox* inbox)
{
    bbVPool_newThreaded(&inbox->local_message_pool,
                 sizeof(bbUI_Inbox_message),
                 1000);

    bbThreadedQueue_init(&inbox->local_message_queue,
                      inbox->local_message_pool,
                      sizeof(bbUI_Inbox_message),
                      1000,offsetof(bbUI_Inbox_message, list_element));

    return bbSuccess;
}


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
        default:

            bbDebug("Unknown UI local message type");

        }
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

    bbDebug("Button unpressed\n");
    return bbSuccess;
}

bbFlag bbUI_Inbox_UnpressButton(bbUI_Inbox* inbox)
{
    bbUI_Inbox_message* message;
    bbThreadedQueue_alloc(&inbox->local_message_queue,(void**)&message);
    message->type = bbUI_Inbox_unpressButton;
    bbThreadedQueue_pushL(&inbox->local_message_queue, (void*)message);
    return bbSuccess;
}