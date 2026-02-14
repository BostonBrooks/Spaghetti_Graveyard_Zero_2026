/** When the user clicks the screen it creates an action.
 * The time the action is started is recorded as well as the time the action
 * should take place. The action takes place a bit after the action is created.
 * This gives the action a chance to traverse the network before taking place.
 *
 * The action is sent to the server and back. Then it is placed on a queue to be
 * executed in the core. There must be a total ordering on actions in the queue.
 * Actions are acted on by the core if the time the action takes place == the
 * current time as observed by the core.
 *
 * If an action reaches the core after the time it should "take place",
 * all of the instructions in the core have to be undone, and actions that should
 * take place after the earlier action should be put back in the sorted queue,
 * back until the time when the action should have taken place.
 * Then, the late received action can be shoe-horned into the core of the game.
 *
 * For starters, we won't implement the shoe-horning algorithm
 */

/* bbActions are created by bbNetworkApp_checkInbox(&home.network); and
 * bbCore_checkLocalMessages(&home.core.core);
 * and those functions put the bbActions in the queue
 *
 * bbFlag bbActions_react(bbActions* actions, bbCore* core,U64 tick_time);
 * takes actions from the queue then adds them to the core, and then reacts the
 * core on each action
*/
#ifndef BB_ACTION_H
#define BB_ACTION_H
#include "engine/core/bbCore.h"
#include "engine/logic/bbHandle.h"
#include "engine/data/CSFML.h"
#include "engine/logic/bbList.h"
#include "engine/logic/bbVPool.h"
#include "engine/threadsafe/bbThreadedQueue.h"

typedef enum
{
    bbActionType_setColor,
} bbAction_type;

typedef  struct
{
    bbListElement_Handle list_element;
    U32 player : 16;
    U32 collision : 16;
    bbAction_type type;
    U64 created_tick;
    U64 act_tick;
    U32 set_color;

} bbAction;

typedef struct
{
    bbVPool* pool;
    bbList future_list;
    //past instructions are stored in pointers in undo instructions.

} bbActions;

///return A < B
I32 bbAction_compare (void* A, void* B);

bbFlag bbActions_init(bbActions* actions);

bbFlag bbAction_setColor(bbActions* actions,
                         U32 color,
                         U32 player,
                         U32 collision,
                         U64 created_tick,
                         U64 act_tick);



#endif // BB_ACTION_H