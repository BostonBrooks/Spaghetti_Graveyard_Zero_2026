#ifndef BB_LOCAL_MESSAGE_INPUTS_H
#define BB_LOCAL_MESSAGE_INPUTS_H
#include "bbCore.h"


bbFlag bbLocalMessage_SetString(bbCore* core, char* string);

bbFlag bbLocalMessage_UnfreezeButton(bbCore* core, char* string);

bbFlag bbLocalMessage_ActionUnfreeze(bbCore* core, char* string);

bbFlag bbLocalMessage_NetsendButton(bbCore* core, char* key);
#endif //BB_LOCAL_MESSAGE_INPUTS_H