#include "engine/core/bbInstruction.h"

typedef enum
{
    bbVInstruction_updateBall = bbInstruction_numTypes,
    bbVInstruction_unupdateBall,

    bbVInstruction_updatePaddle,
    bbVInstruction_unupdatePaddle,

    bbVInstruction_setPaddleDirection,
    bbVInstruction_setPaddleVelocity,
    bbVInstruction_unsetPaddleVelocity,

    bbVInstruction_keyUp,
    bbVInstruction_keyDown,

    bbVInstruction_numTypes,
}bbVInstruction_type;


bbFlag bbVInstruction_updateBall_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdateBall_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_updatePaddle_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unupdatePaddle_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbVInstruction_setPaddleDirection_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_setPaddleVelocity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbVInstruction_unsetPaddleVelocity_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbVInstruction_keyUp_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbVInstruction_keyDown_fn(bbCore* core, bbInstruction* instruction);