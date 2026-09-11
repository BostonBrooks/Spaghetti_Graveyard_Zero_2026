#include "engine/textbox/bbTextbox.h"

#include "engine/logic/bbSystemPool.h"
#include <stddef.h>

bbFlag bbTextbox_systemInit(bbTextbox_system* system)
{
    bbVPool* pool;
    bbVPool_newSystem(&pool, 123,sizeof(bbTextbox_message),10,1000,"TEXTBOXES");

    system->pool = pool;

    return bbSuccess;
}



bbFlag bbTextbox_new(bbTextbox** textbox, bbTextbox_system* system)
{
    bbTextbox* text_box = malloc(sizeof(bbTextbox));
    text_box->system = system;
    bbList_init(&text_box->list,system->pool,NULL,offsetof(bbTextbox_message,list),bbTextbox_sortMessageBy,84);

    *textbox = text_box;

    return bbSuccess;
}


bbFlag bbTextbox_newMessage(bbTextbox *textbox, bbHandle* message_handle, char** message_text)
{
    bbTextbox_system* system = textbox->system;

    bbTextbox_message* message;
    bbHandle messagehandle;

    bbList_alloc2(&textbox->list,(void**)&message,&messagehandle);

    *message_handle = messagehandle;
    *message_text = message->text;
    return bbSuccess;
}


bbFlag bbTextbox_getText(bbTextbox *textbox, bbHandle message_handle, char** message_text)
{
    bbTextbox_system* system = textbox->system;

    bbTextbox_message* message;

    bbVPool_lookup(system->pool,(void**)&message,message_handle);

    *message_text = message->text;

    return bbSuccess;
}


I32 bbTextbox_sortMessageBy(void* A, void* B)
{
    bbTextbox_message* messageA = (bbTextbox_message*)A;
    bbTextbox_message* messageB = (bbTextbox_message*)B;

    return (messageA->timestamp < messageB->timestamp);
}


bbFlag bbTextbox_showMessage(bbTextbox *textbox, bbHandle message_handle, U64 timestamp)
{
    bbTextbox_system* system = textbox->system;
    bbTextbox_message* message;
    bbVPool_lookup(system->pool,(void**)&message,message_handle);
    message->timestamp = timestamp;
    bbList_sortR(&textbox->list,message);

    return bbSuccess;
}


bbFlag bbTextbox_hideMessage(bbTextbox *textbox, bbHandle message_handle)
{
    bbTextbox_system* system = textbox->system;
    bbTextbox_message* message;
    bbVPool_lookup(system->pool,(void**)&message,message_handle);

    bbList_remove(&textbox->list,message);

    return bbSuccess;
}


bbFlag bbTextbox_deleteMessage(bbTextbox *textbox, bbHandle message_handle)
{
    bbTextbox_system* system = textbox->system;
    bbTextbox_message* message;
    bbVPool_lookup(system->pool,(void**)&message,message_handle);

    //TODO If "is in list" function
    if (!bbVPool_handleIsNULL(system->pool,message->list.prev))
    {
        bbList_remove(&textbox->list,message);
    }

    bbVPool_free(system->pool,message);

    return bbSuccess;
}