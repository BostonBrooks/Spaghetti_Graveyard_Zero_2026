/// Functions like bbCore_printString should be called from the same thread
/// that owns the instance of bbCore

#include "engine/core/bbCore.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"

bbFlag bbCore_printInteger(bbCore* core, I32 integer, bool isInput);


bbFlag bbCore_printString(bbCore* core, char* string, bool isInput);
