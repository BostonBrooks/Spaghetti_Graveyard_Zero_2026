#include "engine/core/bbCoreInbox.h"
#include "engine/logic/bbFlag.h"
bbFlag bbCoreInboxLocalMessage_UnfreezeButton(bbCore* core, char* string);

//bbCoreInbox_unfreezeButton in core_inbox.h

bbFlag bbCoreInbox_unfreezeButton_fn(bbCore* core, bbCoreInboxMessage* message);

bbFlag bbCoreInput_unfreezeButton(bbCore* core, char* string, bbInstruction_source source, bbHandle action);

// bbInstruction_unfreezeButton in instructions.h

bbFlag bbInstruction_unfreezeButton_fn(bbCore* core, bbInstruction* instruction);