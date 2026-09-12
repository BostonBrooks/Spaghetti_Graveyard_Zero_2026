#ifndef BB_TEXTBOX_H
#define BB_TEXTBOX_H
#include <pthread.h>

#define MESSAGE_LENGTH 256
#define MESSAGE_BUFFER_LENGTH 256

#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbList.h"
#include "engine/logic/bbVPool.h"

typedef enum
{
    bbSetMessage,
    bbPutMessage,
} bbTextbox_messageType;


typedef struct
{
    U64 timestamp;
    bbListElement_Handle list;
    I32 length;
    bbTextbox_messageType type;
    char text[MESSAGE_LENGTH];
} bbTextbox_message;

typedef struct
{
    bbList list;
    struct bbTextbox_system* system;
    pthread_mutex_t buffer_mutex;
    I32 buffer_start;
    char buffer[MESSAGE_BUFFER_LENGTH];
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

bbFlag bbTextbox_setMessage(bbTextbox *textbox, bbHandle message_handle, U64 timestamp);
bbFlag bbTextbox_putMessage(bbTextbox *textbox, bbHandle message_handle, U64 timestamp);
bbFlag bbTextbox_hideMessage(bbTextbox *textbox, bbHandle message_handle);
bbFlag bbTextbox_deleteMessage(bbTextbox *textbox, bbHandle message_handle);
bbFlag bbTextbox_updateBuffer(bbTextbox *textbox);

#endif //BB_TEXTBOX_H