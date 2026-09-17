#ifndef BB_ACTION_REQUEST_H
#define BB_ACTION_REQUEST_H
#include "engine/core/bbAction.h"
#include "engine/core/bbCore.h"
#include "engine/network/bbNetworkApp.h"


bbFlag bbAction_request(bbCore* core, bbNetwork* network, bbAction* action);

bbFlag bbAction_receive(bbCore* core, bbNetwork* network, bbAction* action);


#endif //BB_ACTION_REQUEST_H