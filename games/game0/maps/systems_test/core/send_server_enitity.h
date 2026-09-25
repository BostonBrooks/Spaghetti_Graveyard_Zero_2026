
#ifndef SEND_SERVER_ENITITY_H
#define SEND_SERVER_ENITITY_H


#include "spawn_entity.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbCoreInbox.h"
#include "engine/core/bbInstruction.h"
#include "engine/data/bbHome.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbHandle.h"
#include "engine/threadsafe/bbThreadedQueue.h"
#include "engine/logic/bbFlag.h"

bbFlag bbCoreInbox_TestClick3(bbCore* core, bbHandle entity_handle);

bbFlag bbCoreInbox_testClick3_fn(bbCore* core, bbCoreInboxMessage* message);

#endif