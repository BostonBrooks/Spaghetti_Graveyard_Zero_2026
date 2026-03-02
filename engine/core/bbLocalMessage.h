#ifndef BB_LOCAL_MESSAGE_H
#define BB_LOCAL_MESSAGE_H
#include "bbInstruction.h"

typedef enum
{
    bbLocalMessage_setString,
    bbLocalMessage_unfreezeButton
} bbLocalMessage_type;

typedef struct
{
    bbLocalMessage_type type;
    bbInstruction_data data;
    bbListElement_Handle list_element;
} bbLocalMessage;



bbFlag bbCore_checkLocalMessages(bbCore* core);

bbFlag bbLocalMessage_setString_fn(bbCore* core, bbLocalMessage* message);

#endif // BB_LOCAL_MESSAGE_H