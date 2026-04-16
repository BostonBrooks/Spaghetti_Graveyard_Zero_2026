#ifndef BB_LOCAL_MESSAGE_H
#define BB_LOCAL_MESSAGE_H
#include "bbInstruction.h"

typedef enum
{
    bbLocalMessage_setString,
    bbLocalMessage_unfreezeButton,
    bbLocalMessage_actionUnfreeze,
    bbLocalMessage_retroactionUnfreeze,
    bbLocalMessage_netsendButton,
    bbLocalMessage_netcodeButton,
    bbLocalMessage_actionLoop,
    bbLocalMessage_netpauseButton,
    bbLocalMessage_keyDown,
    bbLocalMessage_keyUp,
    bbLocalMessage_setViewpoint,
    bbLocalMessage_setGoalpoint
} bbLocalMessage_type;

typedef struct
{
    bbLocalMessage_type type;
    bbInstruction_data data;
    U64 act_time;
    bbListElement_Handle list_element;
} bbLocalMessage;



bbFlag bbCore_checkLocalMessages(bbCore* core);

bbFlag bbLocalMessage_setString_fn(bbCore* core, bbLocalMessage* message);


bbFlag bbLocalMessage_actionUnfreeze_fn(bbCore* core, bbLocalMessage* message);
bbFlag bbLocalMessage_netcodeButton_fn(bbCore* core, bbLocalMessage* message);
bbFlag bbLocalMessage_actionLoop_fn(bbCore* core, bbLocalMessage* message);

bbFlag bbLocalMessage_keyUp_fn(bbCore* core, bbLocalMessage* message);
bbFlag bbLocalMessage_keyDown_fn(bbCore* core, bbLocalMessage* message);


bbFlag bbLocalMessage_setViewpointOut_fn(bbCore* core, bbLocalMessage* message);
bbFlag bbLocalMessage_setGoalpointOut_fn(bbCore* core, bbLocalMessage* message);

#endif // BB_LOCAL_MESSAGE_H