#include "engine/core/bbInstruction.h"

typedef enum
{
    bbVInstruction_setGoalpointOut = bbInstruction_numTypes,
    bbVInstruction_setGoalpointIn,
    bbVInstruction_unsetGoalpoint,

    bbVInstruction_updateMoveables,
    bbVInstruction_unupdateMoveables,

    bbVInstruction_setGoalMoveable,
    bbVInstruction_unsetGoalMoveable,

    bbVInstruction_numTypes
}bbVInstruction_type;


bbFlag bbVInstruction_setGoalpointOut_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_setGoalpointIn_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unsetGoalpoint_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_updateMoveables_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateMoveables_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_setGoalMoveable_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unsetGoalMoveable_fn(bbCore* core, bbInstruction* instruction);

