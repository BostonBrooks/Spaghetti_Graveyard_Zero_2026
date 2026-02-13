/// Functions like bbLocalMessage_printString should be called from a different
/// thread to the one that owns the instance of bbCore

#include "engine/core/bbCore.h"
#include "engine/logic/bbFlag.h"

//I'm not sure if this is a good name
bbFlag bbLocalMessage_PrintString(bbCore* core, char* string);
bbFlag bbLocalMessage_UnfreezeButton(bbCore* core);

bbFlag bbLocalMessage_NetsendButton(bbCore* core);