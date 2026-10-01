#include "engine/ECS/players/bbPlayers_maths.h"

#include "core/actions.h"
#include "core/action_request.h"
#include "core/textboxes.h"
#include "engine/data/bbHome.h"
#include "engine/test_string/bbTestString.h"


bbFlag bbActionRequest_askQuestion(void *Core,
                                   U32 sender,
                                   U32 collision,
                                   bbTime created_tick,
                                   bbTime act_tick,
                                   U32 player,
                                   I32 type,
                                   U64 random_seed)
{
    bbHere()
    bbCore* core = (bbCore*)Core;

    bbAction action;
    action.header.type = bbActionType_askQuestion;
    action.header.status = bbAction_Wait;
    action.header.sender = sender;
    action.header.collision = collision;
    action.header.created_tick = created_tick;
    action.header.act_tick = act_tick;
    action.integer = player;
    action.integer2 = type;
    action.handle.u64 = random_seed;

    bbAction_request(core,&home.network,&action);

    return bbSuccess;
}

bbFlag bbAction_askQuestion_fn(bbCore *core, bbAction *action)
{
    bbHere()
    U32 player_index = action->integer;
    I32 type = action->integer2;
    U64 random_seed = action->handle.u64;
    bbPlayers* players = &home.ECS.players;
    bbNotImplemented() //change the state of players[ player_index]

    bbHandle action_handle;
    bbVPool_reverseLookup(core->action_pool,action,&action_handle);

    bbCoreInput_setPlayerState(&home.core.core, player_index, bbPlayer_state_answeringMaths, bbInstructionSource_action, action_handle);


    if (player_index == players->this_player) {

        U64 random_hash = bbArith64_hash(193193193);
        U64 hashIndex = bbArith64_hashIndex(random_hash, random_seed);
        I32 arg1 = hashIndex % 12;
        I32 arg2 = (hashIndex / 12) %12;
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "Whats is %d + %d?\n", arg1, arg2);
        bbCI_setTextbox(core, buffer, "DIALOGUE?", action->header.act_tick,bbInstructionSource_internal,no_handle);
    }



    return bbSuccess;
}

bbFlag bbActionRequest_answerQuestion(void *Core,
                                      U32 sender,
                                      U32 collision,
                                      bbTime created_tick,
                                      bbTime act_tick,
                                      U32 player,
                                      I32 type,
                                      U64 random_seed,
                                      I32 answer)
{
    bbHere()
    bbCore* core = (bbCore*)Core;

    bbAction action;
    action.header.type = bbActionType_answerQuestion;
    action.header.status = bbAction_Wait;
    action.header.sender = sender;
    action.header.collision = collision;
    action.header.created_tick = created_tick;
    action.header.act_tick = act_tick;
    action.integer = player;
    action.integer2 = type;
    action.integer3 = answer;
    action.handle.u64 = random_seed;
bbDebug("answer = %d\n", answer);
    bbAction_request(core,&home.network,&action);
    return bbSuccess;
}

bbFlag bbAction_answerQuestion_fn(bbCore *core, bbAction *action)
{
    bbHere()
    I32 answer = action->integer3;
    I32 player_int  = action->integer;
    bbDebug("answer = %d\n", answer);
    char buffer[128];

    snprintf(buffer, sizeof(buffer), "Your answer was %d\n", answer);
    if (answer == I32_MIN) {
        bbStr_setStr(buffer,"Your answer was invalid\n",sizeof(buffer));
    }

    bbHandle action_handle;
    bbVPool_reverseLookup(core->action_pool,action,&action_handle);

    bbCoreInput_setPlayerState(&home.core.core, player_int, bbPlayer_state_requestingMaths, bbInstructionSource_action, action_handle);
    bbCI_putTextbox(core, buffer, "DIALOGUE?", action->header.act_tick,bbInstructionSource_internal,no_handle);

    return bbSuccess;
}
