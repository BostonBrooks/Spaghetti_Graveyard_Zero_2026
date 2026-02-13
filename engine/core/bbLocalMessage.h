#ifndef BB_LOCALMESSAGE_H
#define BB_LOCALMESSAGE_H

#include "engine/core/bbInstruction.h"
#include "engine/logic/bbHandle.h"


typedef enum
{
    bbLocalMessage_printString,
    bbLocalMessage_unfreezeButton,
} bbLocalMessage_type;

typedef struct
{
    char string[KEY_LENGTH];
} bbLocalMessage_string;

typedef union
{
    bbInstructionData_integer integer;
    bbInstructionData_string string;
} bbLocalMessage_data;

typedef struct
{
    bbLocalMessage_type type;
    bbLocalMessage_data data;
    bbListElement_Handle list_element;

    //may or may not need to redo local messages
    bbHandle redo;
} bbLocalMessage;


bbFlag bbCore_checkLocalMessages(bbCore* core);
bbFlag bbLocalMessage_printString_fn(bbCore* core, bbLocalMessage* message);
bbFlag bbLocalMessage_unfreezeButton_fn(bbCore* core, bbLocalMessage* message);

#endif // BB_LOCALMESSAGE_H