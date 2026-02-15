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
    instruction->is_action = false;

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
    instruction->is_action = false;
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


bbFlag bbCore_unfreezeButton2(bbCore* core, char* string, bool is_input)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->type = bbInstruction_unfreezeButton2;
    bbStr_setStr(instruction->data.string.string, string, KEY_LENGTH);

    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;

}


bbFlag bbCore_netsendButton(bbCore* core, char* string)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->type = bbInstruction_netsendButton;
    bbStr_setStr(instruction->data.string.string, string, KEY_LENGTH);

    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;
}

///(1) put instruction on do_stack
bbFlag bbCoreInput_setQuote(bbCore* core, char* string, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbInstruction_setQuote;
    bbStr_setStr(instruction->data.string.string, string, KEY_LENGTH);
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}


bbFlag bbCoreInput_setTime(bbCore* core, U64 time, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbInstruction_setTime;
    instruction->data.integer.integer = time;
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}
