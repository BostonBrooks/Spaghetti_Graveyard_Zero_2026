/** The core is responsible for maintaining the state shared between computers
 * in the multplayer game. The core needs to rewind and recalculate state in the
 * event that a network packet comes in late and needs to be dealt with.
 *
 * The core is based on a stack of instructions that are executed in a LIFO order.
 * Instructions are popped and then can puch new instructions to the top of the
 * stack.A second stack is maintained that stores the reverse of the instructions
 * executed, which is used to rewind the game state. In order to fast-forward
 * the game state back to where is was, input instructions are pushed onto the
 * first stack.
 */

#ifndef BB_CORE_H
#define BB_CORE_H
#include "engine/logic/bbList.h"
#include "engine/logic/bbVPool.h"
#include "engine/threadsafe/bbThreadedQueue.h"
#include "engine/core/bbAction.h"

typedef struct
{
    U64 core_time;

    bbVPool* instruction_pool;
    bbList do_stack;
    bbList undo_stack;

    bbVPool* local_message_pool;
    ///Receive messages from GUI, FIFO
    bbThreadedQueue local_message_queue;

    bbVPool* action_pool;
    bbList action_queue;


} bbCore;

bbFlag bbCore_init(bbCore* core);

bbFlag bbCore_react(bbCore* core);
bbFlag bbCore_rewind(bbCore* core);
bbFlag bbCore_clearFuture(bbCore* core);
//bbFlag bbCore_rewindUntil(bbCore* core, bbCallback* callback);
bbFlag bbCore_rewindUntilTime(bbCore* core, U64 time);


#endif // BB_CORE_H

