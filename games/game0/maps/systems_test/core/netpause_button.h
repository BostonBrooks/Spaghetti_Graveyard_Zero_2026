#ifndef NETPAUSE_BUTTON_CODE_H
#define NETPAUSE_BUTTON_CODE_H

#include "engine/core/bbCore.h"
#include "engine/logic/bbFlag.h"

bbFlag bbCoreInbox_NetpauseButton(bbCore* core, char* key);

// enum bbCoreInbox_netpauseButton in core_inbox.h

bbFlag bbCoreInbox_netpauseButton_fn(bbCore* core, bbCoreInboxMessage* message);

bbFlag bbCoreInput_netpauseButton(bbCore* core, char* string);

// enum bbInstruction_netpauseButton in instructions.h

bbFlag bbInstruction_netpauseButton_fn(bbCore* core, bbInstruction* instruction);

#endif // NETPAUSE_BUTTON_CODE_H