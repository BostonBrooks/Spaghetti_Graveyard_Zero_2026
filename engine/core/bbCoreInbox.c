#include "engine/core/bbCoreInbox.h"
#ifdef DEFINE_SKELLYCHASE
#include "core/core_inputs.h"
#endif
#include "engine/core/bbAction.h"
#include "engine/core/bbCoreInputs.h"
#include "engine/data/bbHome.h"
#include "engine/ECS/spawn_entity.h"
#include "engine/threadsafe/bbThreadedQueue.h"

bbFlag bbCoreInbox_setString_fn(bbCore* core, bbCoreInboxMessage* message)
{
    bbCoreInput_setString(core, message->data.key, bbInstructionSource_input, no_handle);
    //undo message?

    return bbSuccess;
}
/*
bbFlag bbCoreInbox_unfreezeButton_fn(bbCore* core, bbCoreInboxMessage* message)
{
    //TODO virtual instruction / callback
    //bbCoreInput_unfreezeButton(core, message->data.key, bbInstructionSource_input, no_handle);


    return bbSuccess;
}

*/
bbFlag bbCoreInbox_testClick_fn(bbCore* core, bbCoreInboxMessage* message)
{

    //TODO virtual instruction / callback
    //bbCoreInput_testClick(core, message->data.map_coords, bbInstructionSource_input, no_handle);
    bbHandle ai_handle;
    bbAI_Component* component;

    bbHandle_mapComponent(home.ECS.ECS, bbECS_ECS,home.ECS.ECS->player_character,
        bbECS_AI,&ai_handle,(bbComponent**)&component);

    bbAI_CommandData data;
    data.goal_point = message->data.map_coords;

    //TODO bbCoreInput

    bbAI_onCommand(component,
                      (bbAI_System*)home.ECS.ECS->systems[bbECS_AI],
                      bbAI_mapClick,
                      data,
                      false);

    return bbSuccess;
}

bbFlag bbCore_checkInbox(bbCore* core)
{
    bbCoreInboxMessage* message;

    bbFlag flag;

    while (1)
    {
        flag = bbThreadedQueue_popR(&core->local_message_queue,(void**)&message);
        if (flag != bbSuccess) return bbSuccess;

        if (message->type >= bbCoreInbox_numTypes)
        {//bbDebug("inbox instruction = %d\n", message->type);
            bbCoreInbox_fn* inbox_fn = core->inbox_functions[message->type-bbCoreInbox_numTypes];

            inbox_fn(core, message);
            bbCore_react(core);
        } else
        {
            switch (message->type)
            {
            case bbCoreInbox_setString:
                bbCoreInbox_setString_fn(core, message);
                bbCore_react(core);
                break;


            default:

                bbDebug("Unknown local message type\n");
            }
        }
    }
}

