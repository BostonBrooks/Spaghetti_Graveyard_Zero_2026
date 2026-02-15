/// Functions like bbCore_printString should be called from the same thread
/// that owns the instance of bbCore

#include "engine/core/bbCore.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"






//the following are too cludgey and will be deprecated
bbFlag bbCore_printInteger(bbCore* core, I32 integer, bool isInput);
bbFlag bbCore_printString(bbCore* core, char* string, bool isInput);
bbFlag bbCore_unfreezeButton(bbCore* core, char* string, bool is_input);
bbFlag bbCore_unfreezeButton2(bbCore* core, char* string, bool is_input);
bbFlag bbCore_netsendButton(bbCore* core,char* string);
