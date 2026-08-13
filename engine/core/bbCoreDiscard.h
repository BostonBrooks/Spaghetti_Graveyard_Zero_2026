#include "engine/core/bbAction.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/core/bbInstruction.h"
#include "engine/core/bbCoreInbox.h"
#include "engine/core/bbCoreInboxInput.h"


///Discard undo instructions up until time, including other objects belonging
///to the undo instructions
bbFlag bbCoreDiscard(bbCore* core, U64 time);

bbFlag bbCore_initDiscard(bbCore* core);