#include <stdlib.h>
#include <stddef.h>
#include "engine/core/bbCore.h"

#include "engine/core/bbCoreInbox.h"
#include "engine/core/bbInstruction.h"
#include "engine/core/bbAction.h"
#include "engine/ECS/bbAI_System.h"
#include "engine/ECS/graphics_system/bbGraphicsSystem.h"
#include "../ECS/server_entities/bbServerEntities.h"
#include "engine/ECS/ECS.h"
#include "engine/ECS/spawn_entity.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/threadsafe/bbThreadedPool.h"
#include "engine/test_string/bbTestString.h"

bbFlag bbCore_init(bbCore* core)
{
    bbVPool_newBloated(&core->instruction_pool, sizeof(bbInstruction), 100, 1000, "bbInstruction");
    bbList_init(&core->do_stack, core->instruction_pool, NULL, offsetof(bbInstruction, list_element),NULL);
    bbList_init(&core->undo_stack, core->instruction_pool, NULL, offsetof(bbInstruction, list_element),NULL);

    bbVPool_newThreaded(&core->local_message_pool, sizeof(bbCoreInboxMessage),1000);

    bbThreadedQueue_init(&core->local_message_queue,
                          core->local_message_pool,
                          sizeof(bbCoreInboxMessage),
                          1000,offsetof(bbCoreInboxMessage, list_element));

    bbVPool_newBloated(&core->action_pool,sizeof(bbAction),100,1000, "bbAction");
    bbList_init(&core->action_queue, core->action_pool, NULL, offsetof(bbAction, header.list_element),bbAction_compare);
    bbList_init(&core->action_temp_fifo, core->action_pool, NULL, offsetof(bbAction, header.list_element),bbAction_compare);

    core->simulation_time = 0;
 return bbSuccess;
}

bbFlag bbCore_react(bbCore* core)
{
    bbFlag flag;
    bbInstruction* instruction;

    while (1)
    {
        flag = bbList_popL(&core->do_stack, (void**)&instruction);
        if (flag != bbSuccess) return bbSuccess;

        if (instruction->type >= bbInstruction_numTypes)
        {
            bbInstruction_fn* instruction_fn = core->instruction_functions[instruction->type-bbInstruction_numTypes];

            bbAssert(instruction_fn != NULL, "Unknown instruction type %d\n", instruction->type);

            instruction_fn(core, instruction);
        } else
        {
            switch (instruction->type)
            {

                ///(2) core reacts to instruction

            case bbInstruction_setTime:
                bbInstruction_setTime_fn(core, instruction);
                break;

            case bbInstruction_setString:
                bbInstruction_setString_fn(core, instruction);
                break;
            case bbI_setString:
                bbI_setString_fn(core, instruction);
                break;

            //case bbI_spawnAIComponent:
                //TODO virtual function / callback
                //bbI_spawnAIComponent_fn(core, instruction);
                //break;

            //case bbInstruction_unfreezeButton:
                //TODO virtual function / callback
                //bbInstruction_unfreezeButton_fn(core, instruction);
                //break;

            case bbInstruction_checkActions:
                bbInstruction_checkActions_fn(core, instruction);
                break;
            //case bbInstruction_spawnEmptyEntity:
                //TODO virtual function / callback
                //bbInstruction_spawnEmptyEntity_fn(core, instruction);
                //break;
            //case bbInstruction_entity_setComponent:
                //TODO virtual function / callback
                //bbInstruction_entity_setComponent_fn(core, instruction);
                //break;
            //case bbInstruction_spawnTestEntity:
                //TODO virtual function / callback
                //bbInstruction_spawnTestEntity_fn(core, instruction);
                //break;

            //case bbInstruction_netpauseButton:
                //TODO virtual function / callback
                //bbInstruction_netpauseButton_fn(core, instruction);
                //break;

            //case bbInstruction_testClick:
                //TODO virtual function / callback
                //bbInstruction_testClick_fn(core, instruction);
                //break;

            ///case bbInstruction_spawnEntityOut:
                //TODO virtual function / callback
                //bbInstruction_spawnEntityOut_fn(core, instruction);
                //break;
            //case bbInstruction_setServerEntity:
                //TODO virtual function / callback
                //bbInstruction_setServerEntity_fn(core, instruction);
                //break;
            //case bbInstruction_spawnGraphicsComponent:
                //TODO virtual function / callback
                //bbInstruction_spawnGraphicsComponent_fn(core, instruction);
                //break;

            //case bbInstruction_updateMoveables:
                //TODO virtual function / callback
                //bbInstruction_updateMoveables_fn(core, instruction);
                //break;

            //case bbInstruction_spawnTestMoveable:
                //TODO virtual function / callback
                //bbInstruction_spawnTestMoveable_fn(core, instruction);
                //break;
            default:
                bbAssert(0==1,"Unknown instruction type %d\n", instruction->type);
            }
        }
    }

    return bbSuccess;
}


bbFlag bbCore_rewindUntil(bbCore* core, U64 time)
{
    bbFlag flag;
    bbInstruction* instruction;

    while (core->simulation_time > time)
    {
        flag = bbList_popL(&core->undo_stack, (void**)&instruction);
        if (flag != bbSuccess) return bbSuccess;

        if (instruction->type >= bbInstruction_numTypes)
        {

            bbInstruction_fn* instruction_fn = core->instruction_functions[instruction->type-bbInstruction_numTypes];
            instruction_fn(core, instruction);

        } else
        {
            switch (instruction->type)
            {
            case bbInstruction_unsetTime:
                bbInstruction_unsetTime_fn(core, instruction);
                break;

                ///(6) core "un-reacts" to instruction
            case bbInstruction_unsetString:
                bbInstruction_unsetString_fn(core, instruction);
                break;

            case bbI_unsetString:
                bbI_unsetString_fn(core, instruction);
                break;

            // case bbI_unspawnAIComponent:
            //     //TODO virtual function / callback
            //     //bbI_unspawnAIComponent_fn(core, instruction);
            //     break;

            case bbInstruction_uncheckActions:
                //TODO virtual function / callback
                //bbInstruction_uncheckActions_fn(core, instruction);
                break;

//             case bbInstruction_unspawnEmptyEntity:
//                 //TODO virtual function / callback
//                 //bbInstruction_unspawnEmptyEntity_fn(core, instruction);
//                 break;
//             case bbInstruction_entity_unsetComponent:
//                 //TODO virtual function / callback
//                 //bbInstruction_entity_unsetComponent_fn(core, instruction);
//                 break;
//             case bbInstruction_unspawnTestEntity:
//                 //TODO virtual function / callback
//                 //bbInstruction_unspawnTestEntity_fn(core, instruction);
//                 break;
//
//             case bbInstruction_unsetServerEntity:
//                 bbHere()
//                 //TODO virtual function / callback
// //bbInstruction_unsetServerEntity_fn(core, instruction);
//                 break;
//             case bbInstruction_unupdateMoveables:
//                 //TODO virtual function / callback
//                 //bbInstruction_unupdateMoveables_fn(core, instruction);
//                 break;
//
//             case bbInstruction_unspawnTestMoveable:
//                 //TODO virtual function / callback
//                 //bbInstruction_unspawnTestMoveable_fn(core, instruction);
//                 break;
//
//             case bbInstruction_unspawnGraphicsComponent:
//                 //TODO virtual function / callback
//                 //bbInstruction_unspawnGraphicsComponent_fn(core, instruction);
//                 break;
            default:
                bbAssert(0==1,"Unknown undo instruction type %d\n", instruction->type);

                bbVPool_free(core->instruction_pool, (void*)instruction);
            }
        }

    }
    return bbSuccess;
}

