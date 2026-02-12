#include "engine/core/bbCoreInputs.h"

#include "bbInstruction.h"



bbFlag bbCore_printInteger(bbCore* core, I32 integer, bool is_input)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->type = bbInstruction_printInteger;
    instruction->data.integer.integer = 193;
    instruction->is_input = is_input;

    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;

}