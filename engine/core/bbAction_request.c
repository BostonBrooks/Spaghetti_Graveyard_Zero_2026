#include "bbCore.h"
#include "engine/core/bbAction.h"
#include "engine/network/bbNetwork.h"
#include "engine/network/bbNetworkApp.h"


bbFlag bbAction_request(bbCore* core, bbNetwork* network, bbAction* action)
{
    if (action->header.status == bbAction_Wait)
    {

        bbNetworkApp_sendAction(network, action);
    } else
    {
        bbNotImplemented()
    }

    return bbSuccess;
}


bbFlag bbAction_receive(bbCore* core, bbNetwork* network, bbAction* action)
{

    if (action->header.status == bbAction_Accept)
    {
        bbAction* new_action;
        bbFlag flag = bbList_alloc(&core->action_queue,(void**)&new_action);

        *new_action = *action;


        bbList_sortL(&core->action_queue,(void*)new_action);
    } else
    {
        bbNotImplemented()
    }

    return bbSuccess;

}