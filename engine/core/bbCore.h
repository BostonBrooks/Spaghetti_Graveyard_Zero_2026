/** The core is responsible for maintaining the state shared between computers
 * in the multiplayer game. The core needs to rewind and recalculate state in the
 * event that a network packet comes in late and needs to be dealt with.
 *
 * The core is based on a stack of instructions that are executed in a LIFO order.
 * Instructions are popped and then can push new instructions to the top of the
 * stack.A second stack is maintained that stores the reverse of the instructions
 * executed, which is used to rewind the game state. In order to fast-forward
 * the game state back to where it was, input instructions are pushed onto the
 * first stack.
 */

#ifndef BB_CORE_H
#define BB_CORE_H

#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbList.h"
#include "engine/logic/bbVPool.h"
#include "engine/threadsafe/bbThreadedQueue.h"

typedef struct bbCore bbCore;
typedef struct bbInstruction bbInstruction;

typedef  bbFlag bbInstruction_fn(bbCore* core, bbInstruction* instruction);

struct bbCore
{
    U64 core_time;

    bbInstruction_fn** instruction_functions;

    bbVPool* instruction_pool;
    bbList do_stack;
    bbList undo_stack;


    bbVPool* local_message_pool;
    ///Receive messages from other threads, FIFO
    bbThreadedQueue local_message_queue;

    bbVPool* action_pool;
    bbList action_queue;
    bbList action_temp_fifo;

    /// Set by bbCore_rewindUntilTime(bbCore* core, U64 time);
    U64 rewind_until_time;
    /// Set by bbCoreInput_setTime()
    U64 simulation_time;
    /// Set by clock / incremented at end of loop
    U64 actual_time;

};

bbFlag bbCore_init(bbCore* core);

bbFlag bbCore_react(bbCore* core);
bbFlag bbCore_rewind(bbCore* core);
bbFlag bbCore_clearFuture(bbCore* core);
//bbFlag bbCore_rewindUntil(bbCore* core, bbCallback* callback);
bbFlag bbCore_rewindUntil(bbCore* core, U64 time);
bbFlag bbCore_initVInstructions(bbCore* core);

#endif // BB_CORE_H

