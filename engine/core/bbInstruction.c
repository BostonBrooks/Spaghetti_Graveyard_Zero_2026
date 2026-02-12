#include "engine/core/bbInstruction.h"

#include "engine/logic/bbTerminal.h"


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