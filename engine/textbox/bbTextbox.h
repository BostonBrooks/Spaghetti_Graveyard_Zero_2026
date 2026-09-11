#ifndef BB_TEXTBOX_H
#define BB_TEXTBOX_H

#define MESSAGE_LENGTH 256

#include "../logic/bbFlag.h"
#include "../logic/bbHandle.h"
#include "../logic/bbList.h"
#include "../logic/bbVPool.h"

typedef struct
{
    U64 timestamp;
    bbListElement_Handle list;
    char text[MESSAGE_LENGTH];
} bbTextbox_message;

typedef struct
{
    bbList list;
    struct bbTextbox_system* system;
    I32 height_chars;
    I32 width_chars;

    pthread_mutex_t buffer_mutex;
    char buffer[MESSAGE_LENGTH*6];
} bbTextbox;

typedef struct bbTextbox_system
{
    bbVPool* pool;
} bbTextbox_system;

bbFlag bbTextbox_systemInit(bbTextbox_system* system);

bbFlag bbTextbox_new(bbTextbox** textbox, bbTextbox_system* system);

//I32 (*compare)(void* A, void* B);
I32 bbTextbox_sortMessageBy(void* A, void* B);

bbFlag bbTextbox_newMessage(bbTextbox *textbox, bbHandle* message_handle, char** message_text);
bbFlag bbTextbox_getText(bbTextbox *textbox, bbHandle message_handle, char** message_text);

bbFlag bbTextbox_showMessage(bbTextbox *textbox, bbHandle message_handle, U64 timestamp);
bbFlag bbTextbox_hideMessage(bbTextbox *textbox, bbHandle message_handle);
bbFlag bbTextbox_deleteMessage(bbTextbox *textbox, bbHandle message_handle);
bbFlag bbTextbox_updateBuffer(bbTextbox *textbox);

#endif //BB_TEXTBOX_H