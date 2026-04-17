
#ifndef BB_INSTRUCTION_H
#define BB_INSTRUCTION_H
#include "engine/core/bbCore.h"
#include "engine/data/bbConstants.h"
#include "engine/geometry/bbCoordinates.h"
#include "engine/logic/bbHandle.h"

typedef enum
{
    bbInstruction_setString,
    bbInstruction_unsetString,


    bbInstruction_checkActions,
    bbInstruction_uncheckActions,

    bbInstruction_setTime,
    bbInstruction_unsetTime,

    bbInstruction_unfreezeButton,
    bbInstruction_netsendButton,
    bbInstruction_netcodeButton,
    bbInstruction_loopAction,
    bbInstruction_netpauseButton,

    bbInstruction_updateBall,
    bbInstruction_unupdateBall,

    bbInstruction_updatePaddle,
    bbInstruction_unupdatePaddle,


    bbInstruction_keyUp,
    bbInstruction_keyDown,

    bbInstruction_setPaddleDirection,
    bbInstruction_setPaddleVelocity,
    bbInstruction_unsetPaddleVelocity,

    bbInstruction_setViewpointOut,
    bbInstruction_setViewpointIn,
    bbInstruction_unsetViewpoint,

    bbInstruction_setGoalpointOut,
    bbInstruction_setGoalpointIn,
    bbInstruction_unsetGoalpoint,
    bbInstruction_approachGoalpoint,
    bbInstruction_unapproachGoalpoint
} bbInstruction_type;


typedef enum
{
    ///The instruction was added from inside the core
    bbInstructionSource_internal,
    ///The instruction was added in the MAIN thread
    bbInstructionSource_input,
    ///The instruction was added through the action queue
    bbInstructionSource_action,
} bbInstruction_source;

typedef struct
{
    bbHandle handle1;
    bbHandle handle2;
    bbHandle handle3;
} bbThreeHandles;

typedef union
{
    char string[KEY_LENGTH];
    U64 unsigned_long;
    bbThreeHandles three_handles;
    bbMapCoords map_coords;
} bbInstruction_data;

typedef struct
{
    bbInstruction_type type;
    bbInstruction_data data;
    U64 act_time;
    U32 collision;
    bbListElement_Handle list_element;
    bbHandle redo_instruction;
    bbInstruction_source source;
} bbInstruction;


bbFlag bbInstruction_checkActions_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_uncheckActions_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbInstruction_checkActions2_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_uncheckActions2_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbInstruction_updateBall_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unupdateBall_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbInstruction_updatePaddle_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unupdatePaddle_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbInstruction_setPaddleDirection_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_setPaddleVelocity_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unsetPaddleVelocity_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbInstruction_setString_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unsetString_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbInstruction_setTime_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unsetTime_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbInstruction_unfreezeButton_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_netsendButton_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_netcodeButton_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_loopAction_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_netpauseButton_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbInstruction_keyUp_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_keyDown_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbInstruction_setViewpointOut_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_setViewpointIn_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unsetViewpoint_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbInstruction_setGoalpointOut_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_setGoalpointIn_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_approachGoalpoint_fn(bbCore* core, bbInstruction* instruction);


#endif //BB_INSTRUCTION_H