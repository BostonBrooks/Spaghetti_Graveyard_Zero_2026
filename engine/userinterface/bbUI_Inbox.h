#ifndef BB_UI_INBOX_H
#define BB_UI_INBOX_H

#include "engine/core/bbLocalMessage.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"
#include "engine/threadsafe/bbThreadedPool.h"

typedef enum
{
    bbUI_Inbox_unpressButton,
    bbUI_Inbox_unpressButton2,
} bbUI_Inbox_type;

typedef struct
{
    bbHandle handle;
} bbUI_Inbox_handle;

typedef struct
{
    char string[KEY_LENGTH];
} bbUI_Inbox_string;

typedef union
{
    bbUI_Inbox_handle handle;
    bbUI_Inbox_string string;
} bbUI_Inbox_data;

typedef struct
{
    bbUI_Inbox_type type;
    bbUI_Inbox_data data;
    bbListElement_Handle list_element;

    //may or may not need to redo local messages
    bbHandle redo;
} bbUI_Inbox_message;

typedef struct
{
    bbVPool* local_message_pool;
    ///Receive messages from GUI, FIFO
    bbThreadedQueue local_message_queue;
} bbUI_Inbox;

bbFlag bbUI_Inbox_init(bbUI_Inbox* inbox);

bbFlag bbUI_Inbox_check(bbUI_Inbox* inbox);
bbFlag bbUI_Inbox_unpressButton_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message);
bbFlag bbUI_Inbox_unpressButton2_fn(bbUI_Inbox* inbox, bbUI_Inbox_message* message);


bbFlag bbUI_Inbox_UnpressButton(bbUI_Inbox* inbox);
bbFlag bbUI_Inbox_UnpressButton2(bbUI_Inbox* inbox, char* key);
#endif // BB_UI_INBOX_H