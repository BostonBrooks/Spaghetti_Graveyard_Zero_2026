

#include <stdlib.h>

#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/logic/bbFlag.h"
#include "instructions.h"

bbFlag bbCore_initVInstructions(bbCore* core)
{
    core->instruction_functions
        = calloc(bbVInstruction_numTypes-bbInstruction_numTypes,
            sizeof (bbInstruction_fn*));

    core->instruction_functions[bbVInstruction_updateBall-bbInstruction_numTypes]
        = bbVInstruction_updateBall_fn;

    core->instruction_functions[bbVInstruction_unupdateBall-bbInstruction_numTypes]
        = bbVInstruction_unupdateBall_fn;

    core->instruction_functions[bbVInstruction_updatePaddle-bbInstruction_numTypes]
        = bbVInstruction_updatePaddle_fn;

    core->instruction_functions[bbVInstruction_unupdatePaddle-bbInstruction_numTypes]
        = bbVInstruction_unupdatePaddle_fn;

    core->instruction_functions[bbVInstruction_setPaddleDirection-bbInstruction_numTypes]
        = bbVInstruction_setPaddleDirection_fn;

    core->instruction_functions[bbVInstruction_setPaddleVelocity-bbInstruction_numTypes]
        = bbVInstruction_setPaddleVelocity_fn;

    core->instruction_functions[bbVInstruction_unsetPaddleVelocity-bbInstruction_numTypes]
        = bbVInstruction_unsetPaddleVelocity_fn;
}
