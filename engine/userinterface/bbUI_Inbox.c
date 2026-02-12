#include "engine/userinterface/bbUI_Inbox.h"

bbFlag bbUI_Inbox_init(bbUI_Inbox* inbox)
{
    bbVPool_newThreaded(&inbox->local_message_pool, sizeof(bbUI_Inbox_message),1000);

    bbThreadedQueue_init(&core->local_message_queue,
                      core->local_message_pool,
                      sizeof(bbLocalMessage),
                      1000,offsetof(bbLocalMessage, list_element));

    return bbSuccess;
}

