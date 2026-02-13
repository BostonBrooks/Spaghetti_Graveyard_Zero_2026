#include "engine/core/bbCoreInputs.h"

#include "bbInstruction.h"
#include "engine/logic/bbString.h"


bbFlag bbCore_printInteger(bbCore* core, I32 integer, bool is_input)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->type = bbInstruction_printInteger;
    instruction->data.integer.integer = integer;
    instruction->is_input = is_input;

    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;

}

bbFlag bbCore_printString(bbCore* core, char* string, bool is_input)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->type = bbInstruction_printString;
    bbStr_setStr(instruction->data.string.string, string, KEY_LENGTH);
    instruction->is_input = is_input;

    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;

}

bbFlag bbCore_unfreezeButton(bbCore* core, char* string, bool is_input)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->type = bbInstruction_unfreezeButton;

    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;

}