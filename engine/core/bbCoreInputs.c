#include "bbCore.h"
#include "bbCoreInbox.h"
#include "bbInstruction.h"

#ifdef DEFINE_PONG
#include "core/instructions.h"
#endif

#ifdef DEFINE_SKELLYCHASE
#include "core/instructions.h"
#endif


#include "engine/logic/bbFlag.h"
#include "engine/logic/bbString.h"

bbFlag bbCoreInput_setString(bbCore* core, char* string, bbInstruction_source source, bbHandle action)
{

    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbInstruction_setString;
    bbStr_setStr(instruction->data.key, string, KEY_LENGTH);
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}


bbFlag bbCoreInput_checkActions(bbCore* core, U64 time, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbInstruction_checkActions;
    instruction->source = source;
    instruction->redo_instruction = action;
    instruction->data.u64 = time;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}


bbFlag bbCoreInput_setTime(bbCore* core, U64 time, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbInstruction_setTime;
    instruction->data.u64 = time;
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}

bbFlag bbCoreInput_netpauseButton(bbCore* core, char* string)
{
    bbInstruction* instruction;
    bbList_alloc(&core->do_stack, (void**) &instruction);

    instruction->type = bbInstruction_netpauseButton;
    bbStr_setStr(instruction->data.key, string, KEY_LENGTH);

    bbList_pushL(&core->do_stack, instruction);

    return bbSuccess;
}

bbFlag bbCoreInput_unfreezeButton(bbCore* core, char* string, bbInstruction_source source, bbHandle action)
{
    bbInstruction* instruction;
    bbFlag flag = bbList_alloc(&core->do_stack,(void**)&instruction);

    instruction->type = bbInstruction_unfreezeButton;
    bbStr_setStr(instruction->data.key, string, KEY_LENGTH);
    instruction->source = source;
    instruction->redo_instruction = action;

    bbList_pushL(&core->do_stack, instruction);
    return bbSuccess;
}


bbFlag bbCoreInput_testClick(bbCore* core, bbMapCoords MC, bbInstruction_source source, bbHandle action)
{
    bbHere()
}