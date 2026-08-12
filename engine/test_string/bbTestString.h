#ifndef BB_TESTSTRING_H
#define BB_TESTSTRING_H

#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/data/bbConstants.h"
#include "engine/logic/bbFlag.h"

extern char test_string[KEY_LENGTH];

// typedef enum
// {
//     bbI_setString,
//     bbI_unsetString,
//     ...
// }

bbFlag bbI_setString_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_unsetString_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbCI_setString(bbCore* core, char* string, bbInstruction_source source, bbHandle action);
bbFlag bbCS_setString(bbCore* core, char* string, bbInstruction_source source, bbHandle action);

bbFlag bbA_setString(void* Core,
                            U32 player,
                            U32 collision,
                            U64 created_tick,
                            U64 act_tick,
                            char* key);

#endif //BB_TESTSTRING_H