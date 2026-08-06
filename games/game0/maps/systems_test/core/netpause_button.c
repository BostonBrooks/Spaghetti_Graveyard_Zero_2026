#include "core_inbox.h"
#include "netpause_button.h"
#include "instructions.h"
#include "engine/data/bbHome.h"
#include "engine/logic/bbString.h"
#include "engine/network/bbNetworkApp.h"

bbFlag bbCoreInbox_NetpauseButton(bbCore* core, char* key)
{
        bbCoreInboxMessage* message;
        bbThreadedQueue_alloc(&core->local_message_queue, (void**)&message);
        message->type = bbCoreInbox_netpauseButton;

        bbStr_setStr(message->data.key, key, KEY_LENGTH);

        bbThreadedQueue_pushL(&core->local_message_queue, message);
        return bbSuccess;

}

// enum bbCoreInbox_netpauseButton in core_inbox.h

bbFlag bbCoreInbox_netpauseButton_fn(bbCore* core, bbCoreInboxMessage* message)
{
        bbCoreInput_netpauseButton(core,message->data.key);

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

// enum bbInstruction_netpauseButton in instructions.h

bbFlag bbInstruction_netpauseButton_fn(bbCore* core, bbInstruction* instruction)
{
        bbNetworkApp_netpauseButton(&home.network, instruction->data.key);
        return bbSuccess;
}