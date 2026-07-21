#ifndef BB_LOCAL_MESSAGE_INPUTS_H
#define BB_LOCAL_MESSAGE_INPUTS_H
#include "engine/core/bbCore.h"


bbFlag bbCoreInbox_SetString(bbCore* core, char* string);

bbFlag bbCoreInbox_UnfreezeButton(bbCore* core, char* string);

bbFlag bbCoreInbox_TestClick(bbCore* core, bbMapCoords MC);
#endif //BB_LOCAL_MESSAGE_INPUTS_H