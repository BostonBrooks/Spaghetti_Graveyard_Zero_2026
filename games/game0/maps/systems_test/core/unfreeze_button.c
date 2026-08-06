#include "engine/core/bbCoreInbox.h"
#include "engine/logic/bbString.h"
#include "engine/threadsafe/bbThreadedQueue.h"
#include "unfreeze_button.h"

#include "core_inbox.h"
#include "instructions.h"
#include "engine/data/bbHome.h"
#include "engine/userinterface/bbUI_Inbox.h"

bbFlag bbCoreInbox_UnfreezeButton(bbCore* core, char* string)
{
    bbCoreInboxMessage* message;
    bbThreadedQueue_alloc(&core->local_message_queue, (void** ) &message);
    message->type = bbCoreInbox_unfreezeButton;
    bbStr_setStr(message->data.key, string, KEY_LENGTH);
    bbThreadedQueue_pushL(&core->local_message_queue, message);
    return bbSuccess;
}
bbFlag bbCoreInbox_unfreezeButton_fn(bbCore* core, bbCoreInboxMessage* message)
{
    bbCoreInput_unfreezeButton(core, message->data.key, bbInstructionSource_input, no_handle);
    //undo message?

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

bbFlag bbInstruction_unfreezeButton_fn(bbCore* core, bbInstruction* instruction)
{

    bbDebug("unfreeze button %s\n", instruction->data.key);

    bbUI_Inbox_UnpressButton2(&home.UI.inbox, instruction->data.key);

    return bbSuccess;
}