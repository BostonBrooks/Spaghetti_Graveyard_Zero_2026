#include <stdlib.h>
#include <stddef.h>
#include "engine/core/bbCore.h"

#include "bbInstruction_operations.h"
#include "../../games/game0/maps/systems_test/core/action_request.h"
#include "engine/core/bbCoreInbox.h"
#include "engine/core/bbInstruction.h"
#include "engine/core/bbAction.h"
#include "../ECS/AI_system/bbAI_System.h"
#include "engine/ECS/graphics_system/bbGraphicsSystem.h"
#include "../ECS/server_entities/bbServerEntities.h"
#include "engine/ECS/bbECS.h"
#include "games/game0/maps/systems_test/core/spawn_entity.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/threadsafe/bbThreadedPool.h"
#include "engine/test_string/bbTestString.h"
#include "engine/core/bbAction_check.h"
#include "engine/logic/bbSegmentedDeque.h"

bbFlag bbCore_init(bbCore* core)
{
    bbVPool_newBloated(&core->instruction_pool, sizeof(bbInstruction), 1000, 10, "bbInstruction");
    bbList_init(&core->active_stack, core->instruction_pool, NULL, offsetof(bbInstruction, list_element),NULL,69);
    bbList_init(&core->undo_stack, core->instruction_pool, NULL, offsetof(bbInstruction, list_element),NULL,70);

    bbVPool_newThreaded(&core->local_message_pool, sizeof(bbCoreInboxMessage),1000);

    bbThreadedQueue_init(&core->local_message_queue,
                          core->local_message_pool,
                          sizeof(bbCoreInboxMessage),
                          1000,offsetof(bbCoreInboxMessage, list_element));

    bbVPool_newBloated(&core->action_pool,sizeof(bbAction),10,1000, "bbAction");
    bbList_init(&core->action_queue, core->action_pool, NULL, offsetof(bbAction, header.list_element),bbAction_compare,71);
    bbList_init(&core->action_temp_fifo, core->action_pool, NULL, offsetof(bbAction, header.list_element),bbAction_compare,72);

    bbInstruction_deque_init(&core->active_instructions,1024, "Active Instructions");
    bbInstruction_deque_init(&core->undo_instructions,1024,"Undo Instructions");
    bbInstruction_deque_init(&core->redo_instructions,1024,"Redo Instructions");

    core->simulation_time = 0;
 return bbSuccess;
}

bbFlag bbCore_react(bbCore* core)
{//printf("+++\n");
    while (1)
    {//bbHere()

        fetchInstruction(core)

        //bbHere()

        if (instruction.type >= bbInstruction_numTypes)
        {//bbHere()
            bbInstruction_fn* instruction_fn = core->instruction_functions[instruction.type-bbInstruction_numTypes];

            //bbDebug("instruction type = %d\n", instruction.type);
            bbAssert(instruction_fn != NULL, "Unknown instruction type %d\n", instruction.type);

            instruction_fn(core, &instruction);
        } else
        {
            //bbDebug("instruction type = %d\n", instruction.type);
            switch (instruction.type)
            {

                ///(2) core reacts to instruction

            case bbInstruction_setTime:
                ///bbHere()
                bbInstruction_setTime_fn(core, &instruction);
                //bbHere()
                break;

            // case bbInstruction_setString:
            //     bbInstruction_setString_fn(core, instruction);
            //     break;

            case bbInstruction_setString:
            case bbI_setString:
                //bbHere()
                bbI_setString_fn(core, &instruction);
                break;


            case bbI_doNothing:
                bbI_doNothing_fn(core, &instruction);
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
                //bbHere()
                bbInstruction_checkActions_fn(core, &instruction);
                //bbHere()
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
                bbAssert(0==1,"Unknown instruction type %d\n", &instruction.type);
            }
        }
    }

    return bbSuccess;
}


bbFlag bbCore_rewindUntil(bbCore* core, bbTime time)
{//bbHere()
    while (core->simulation_time > time)
    { //bbHere()


        fetchUndoInstruction(core)

        if (instruction.type >= bbInstruction_numTypes)
        {

            bbInstruction_fn* instruction_fn = core->instruction_functions[instruction.type-bbInstruction_numTypes];
            //bbHere()
            instruction_fn(core, &instruction);
            //bbHere()

        } else
        {
            switch (instruction.type)
            {
            case bbInstruction_unsetTime:
                //bbHere()
                bbInstruction_unsetTime_fn(core, &instruction);
                //bbHere()
                break;

                ///(6) core "un-reacts" to instruction
            // case bbInstruction_unsetString:
            //     bbInstruction_unsetString_fn(core, instruction);
            //     break;

            case bbI_unsetString:
            case bbInstruction_unsetString:
                //bbHere()
                bbI_unsetString_fn(core, &instruction);
                //bbHere()
                break;

            case bbI_undoNothing:
                //bbHere()
                bbI_undoNothing_fn(core, &instruction);
                //bbHere()
                break;

            // case bbI_unspawnAIComponent:
            //     //TODO virtual function / callback
            //     //bbI_unspawnAIComponent_fn(core, instruction);
            //     break;

            case bbInstruction_uncheckActions:
                //bbHere()
                bbInstruction_uncheckActions_fn(core, &instruction);
                //bbHere()
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
                    bbInstruction* redo_instruction = instruction.redo_instruction.ptr;

                bbDebug(0==1,"Unknown undo instruction type %d\n"
                    "redo instruction type %d\n instruction source %d\n", instruction.type,redo_instruction->type, instruction.source);

                //bbVPool_free(core->instruction_pool, (void*)instruction_ptr);
            }
        }

    }
    //bbHere()
    return bbSuccess;
}


bbFlag bbPrintStack_fn(bbList* list, void* node, void* cl)
{
    bbInstruction* instruction = (bbInstruction*)node;
    printf("instruction = %d\n", (instruction->type));

    return bbContinue;
}
bbFlag bbCore_printStack(bbCore* core)
{
    printf("Do Stack:\n");
    bbList_mapL(&core->active_stack, bbPrintStack_fn, NULL);


    printf("Undo Stack:\n");
    bbList_mapL(&core->undo_stack, bbPrintStack_fn, NULL);

    return bbSuccess;
}

DECLARE_SQ_BODY(bbInstruction,sizeof(bbInstruction),169)