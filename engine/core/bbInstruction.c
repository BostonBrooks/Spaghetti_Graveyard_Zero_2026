#include "engine/core/bbInstruction.h"

#include "engine/data/bbHome.h"
#include "engine/logic/bbTerminal.h"
#include "engine/userinterface/bbUI_Inbox.h"


bbFlag bbInstruction_printInteger_fn(bbCore* core, bbInstruction* instruction)
{
    bbDebug("print integer: %d\n", instruction->data.integer.integer);
    return bbSuccess;
}
bbFlag bbInstruction_unprintInteger_fn(bbCore* core, bbInstruction* instruction)
{
    bbDebug("unprint integer: %d\n", instruction->data.integer.integer);
    return bbSuccess;
}

bbFlag bbInstruction_printString_fn(bbCore* core, bbInstruction* instruction)
{
    bbDebug("print string: %s\n", instruction->data.string.string);
    bbUI_Inbox_UnpressButton(&home.UI.inbox);
    return bbSuccess;
}
bbFlag bbInstruction_unprintString_fn(bbCore* core, bbInstruction* instruction)
{
    bbDebug("unprint string: %s\n", instruction->data.string.string);
    return bbSuccess;
}