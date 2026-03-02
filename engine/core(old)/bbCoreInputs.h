/// Functions like bbCore_printString should be called from the same thread
/// that owns the instance of bbCore

#include "bbInstruction.h"
#include "engine/core/bbCore.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"






//the following are too cludgey and will be deprecated
bbFlag bbCoreInput_printInteger(bbCore* core, I32 integer, bool isInput);
bbFlag bbCoreInput_printString(bbCore* core, char* string, bool isInput);
bbFlag bbCoreInput_unfreezeButton(bbCore* core, char* string, bool is_input);
bbFlag bbCoreInput_unfreezeButton2(bbCore* core, char* string, bool is_input);
bbFlag bbCoreInput_netsendButton(bbCore* core,char* string);
bbFlag bbCoreInput_netpauseButton(bbCore* core,char* string);

bbFlag bbCoreInput_netcodeButton(bbCore* core, char* string, U64 time,
                                  bbInstruction_source source, bbHandle action);


bbFlag bbCoreInput_loop(bbCore* core, char* string, U64 time,
                                  bbInstruction_source source, bbHandle action);

///Place an instruction in the core to send a message to the UI to set the text
///of the prompt widget. Instruction places reverse instruction on the undo stack.
///if instruction source is internal, no need to redo
///if instruction source is input, place original instruction in reverse
///instruction's redo handle
///if instruction source is action, put action argument in undo instruction's redo handle
///for now we'll ignore the action case

bbFlag bbCoreInput_setQuote(bbCore* core, char* string, bbInstruction_source source, bbHandle action);
bbFlag bbCoreInput_setTime(bbCore* core, U64 time, bbInstruction_source source, bbHandle action);


bbFlag bbCoreInput_checkActions(bbCore* core, U64 time, bbInstruction_source source, bbHandle action);



bbFlag bbCoreInput_setTestInt(bbCore* core, U64 time, bbInstruction_source source, bbHandle action);