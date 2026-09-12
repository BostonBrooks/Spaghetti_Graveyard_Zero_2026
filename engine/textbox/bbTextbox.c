#include "engine/textbox/bbTextbox.h"

#include "engine/logic/bbSystemPool.h"
#include <stddef.h>

#include "engine/logic/bbIterator.h"
#include "engine/logic/bbString.h"

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
    pthread_mutex_init(&text_box->buffer_mutex, NULL);
    text_box->buffer_start = MESSAGE_BUFFER_LENGTH-1;
    text_box->buffer[MESSAGE_BUFFER_LENGTH-1] = '\0';

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


bbFlag bbTextbox_setMessage(bbTextbox *textbox, bbHandle message_handle, U64 timestamp)
{
    bbTextbox_system* system = textbox->system;
    bbTextbox_message* message;
    bbVPool_lookup(system->pool,(void**)&message,message_handle);
    message->timestamp = timestamp;
    message->type = bbSetMessage;
    message->length = strlen(message->text);
    bbList_sortR(&textbox->list,message);

    return bbSuccess;
}

bbFlag bbTextbox_putMessage(bbTextbox *textbox, bbHandle message_handle, U64 timestamp)
{
    bbTextbox_system* system = textbox->system;
    bbTextbox_message* message;
    bbVPool_lookup(system->pool,(void**)&message,message_handle);
    message->timestamp = timestamp;
    message->type = bbPutMessage;
    message->length = strlen(message->text);
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

    if (message->list.list_id != 0)
    {
        bbList_remove(&textbox->list,message);
    }

    bbVPool_free(system->pool,message);

    return bbSuccess;
}

bbFlag bbTextbox_updateBuffer(bbTextbox *textbox)
{
    bbMutexLock(&textbox->buffer_mutex);
    textbox->buffer_start = MESSAGE_BUFFER_LENGTH-1;
    bbTextbox_message* message;
    bbIterator iterator = bbIterator_new(&textbox->list);
    bbFlag flag = bbIterator_setTail(&iterator,NULL,(void**)&message);
    //bbFlag flag = bbList_peakR(&textbox->list,(void**)&message);
    bbFlag flag2 = bbSuccess;
    I32 messageLength = message->length;
    while (message->text[messageLength-1] == '\n'){ messageLength--;}
    while (1)
    {
        flag = bbStr_copyBack(textbox->buffer,&textbox->buffer_start,message->text,messageLength);
        if (flag != bbSuccess) break;
        flag2 = bbIterator_decrement(&iterator,NULL,(void**)&message);
        if (flag2 != bbSuccess) break;
        messageLength = message->length;
    }

    textbox->buffer[MESSAGE_BUFFER_LENGTH-1] = '\0';
    bbMutexUnlock(&textbox->buffer_mutex);
    return bbSuccess;
}

///not threadsafe! update buffer needs to be called from the thread that owns the system!
bbFlag bbTextbox_updatecopyBuffer(bbTextbox *textbox, char* new_buffer, I32 rows, I32 columns, I32 max)
{
    bbMutexLock(&textbox->buffer_mutex);

    textbox->buffer_start = MESSAGE_BUFFER_LENGTH-1;
    bbTextbox_message* message;
    bbIterator iterator = bbIterator_new(&textbox->list);
    bbFlag flag = bbIterator_setTail(&iterator,NULL,(void**)&message);
    //bbFlag flag = bbList_peakR(&textbox->list,(void**)&message);
    bbFlag flag2 = bbSuccess;
    I32 messageLength = message->length;
    while (message->text[messageLength-1] == '\n'){ messageLength--;}
    while (1)
    {
        flag = bbStr_copyBack(textbox->buffer,&textbox->buffer_start,message->text,messageLength);
        if (flag != bbSuccess) break;
        flag2 = bbIterator_decrement(&iterator,NULL,(void**)&message);
        if (flag2 != bbSuccess) break;
        messageLength = message->length;
    }

    textbox->buffer[MESSAGE_BUFFER_LENGTH-1] = '\0';

    bbStr_copyBounds(new_buffer,&textbox->buffer[textbox->buffer_start],columns,rows,max);

    bbMutexUnlock(&textbox->buffer_mutex)
    return bbSuccess;
}


bbFlag bbTextbox_copyBuffer(bbTextbox *textbox, char* new_buffer, I32 rows, I32 columns, I32 max)
{
    bbMutexLock(&textbox->buffer_mutex);

    bbStr_copyBounds(new_buffer,&textbox->buffer[textbox->buffer_start],columns,rows,max);

    bbMutexUnlock(&textbox->buffer_mutex)
    return bbSuccess;
}