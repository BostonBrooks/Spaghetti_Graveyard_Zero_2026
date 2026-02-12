///A bunch of functions that put instructions on the stack, to be executed
///Not threadsafe

#include "engine/core/bbCore.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"

bbFlag bbCore_printInteger(bbCore* core, I32 integer, bool isInput);


bbFlag bbCore_printString(bbCore* core, char* string, bool isInput);
