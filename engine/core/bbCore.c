#include <stddef.h>
#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/logic/bbBloatedPool.h"
#include "engine/threadsafe/bbThreadedPool.h"
#include "engine/core/bbLocalMessage.h"
#include "engine/logic/bbTerminal.h"

bbFlag bbCore_init(bbCore* core)
{
    bbVPool_newBloated(&core->instruction_pool, sizeof(bbInstruction), 100, 1000);
    bbList_init(&core->do_stack, core->instruction_pool, NULL, offsetof(bbInstruction, list_element),NULL);
    bbList_init(&core->undo_stack, core->instruction_pool, NULL, offsetof(bbInstruction, list_element),NULL);
    bbVPool_newThreaded(&core->local_message_pool, sizeof(bbLocalMessage),1000);

    bbThreadedQueue_init(&core->local_message_queue,
                          core->local_message_pool,
                          sizeof(bbLocalMessage),
                          1000,offsetof(bbLocalMessage, list_element));

    bbHere();
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

        switch (instruction->type)
        {
        case bbInstruction_printInteger:
            bbInstruction_printInteger_fn(core, instruction);
            break;

        case bbInstruction_printString:
            bbInstruction_printString_fn(core, instruction);
            break;

        case bbInstruction_unfreezeButton:
            bbInstruction_unfreezeButton_fn(core, instruction);
            break;
        case bbInstruction_netsendButton:
            bbDebug("Send button click to server\n");
            break;
        default:
            bbDebug("Unknown instruction type");
        }

    }

    return bbSuccess;
}

bbFlag bbCore_rewind(bbCore* core)
{
    bbFlag flag;
    bbInstruction* instruction;

    while (1)
    {
        flag = bbList_popL(&core->undo_stack, (void**)&instruction);
        if (flag != bbSuccess) return bbSuccess;

        switch (instruction->type)
        {
        case bbInstruction_unprintInteger:
            bbInstruction_unprintInteger_fn(core, instruction);
            break;
        case bbInstruction_unprintString:
            bbInstruction_unprintString_fn(core, instruction);
            break;
        default:
            bbDebug("Unknown undo instruction type");
        }

    }
    return bbSuccess;
}


