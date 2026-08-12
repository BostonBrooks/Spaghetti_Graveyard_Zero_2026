#ifndef BB_ACTION_CHECK
#define BB_ACTION_CHECK


#include "bbAction.h"
#include "bbCore.h"
#include "engine/logic/bbFlag.h"
#include "engine/core/bbInstruction.h"
#include "engine/test_string/bbTestString.h"

bbFlag bbInstruction_checkActions_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbInstruction_uncheckActions_fn(bbCore* core, bbInstruction* instruction);

#endif //BB_BB_ACTION_CHECK