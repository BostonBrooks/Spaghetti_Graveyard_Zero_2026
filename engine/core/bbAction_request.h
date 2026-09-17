#ifndef BB_ACTION_REQUEST_H
#define BB_ACTION_REQUEST_H
#include "engine/core/bbAction.h"
#include "engine/core/bbCore.h"
#include "engine/network/bbNetworkApp.h"
#include "engine/core/bbInstruction.h"

bbFlag bbAction_request(bbCore* core, bbNetwork* network, bbAction* action);

bbFlag bbAction_receive(bbCore* core, bbNetwork* network, bbAction* action);

bbFlag bbCoreInput_requestAction(bbCore* core,
                                  bbNetwork* network,
                                  bbAction* new_action,
                                  U64 time,
                                  bbInstruction_source source,
                                  bbHandle action);


bbFlag bbInstruction_requestAction_fn(bbCore* core, bbInstruction* instruction);



#endif //BB_ACTION_REQUEST_H