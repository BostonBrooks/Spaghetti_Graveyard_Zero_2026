#ifndef BBPLAYERS_MATHS_H
#define BBPLAYERS_MATHS_H

#include "engine/core/bbCore.h"
#include "engine/logic/bbFlag.h"

bbFlag bbActionRequest_askQuestion(void* Core,
                       U32 sender,
                       U32 collision,
                       bbTime created_tick,
                       bbTime act_tick,
                       U32 player,
                       I32 type,
                       U64 random_seed);

bbFlag bbAction_askQuestion_fn(bbCore* core, bbAction* action);

bbFlag bbActionRequest_answerQuestion(void* Core,
                       U32 sender,
                       U32 collision,
                       bbTime created_tick,
                       bbTime act_tick,
                       U32 player,
                       I32 type,
                       U64 random_seed,
                       I32 answer);

bbFlag bbAction_answerQuestion_fn(bbCore* core, bbAction* action);



#endif //BBPLAYERS_MATHS_H