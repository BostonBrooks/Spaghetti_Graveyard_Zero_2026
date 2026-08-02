
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

    bbI_setString,
    bbI_unsetString,

    //bbI_spawnAIComponent,
    //bbI_unspawnAIComponent,

    //bbInstruction_unfreezeButton,
    //bbInstruction_testClick,

    bbInstruction_checkActions,
    bbInstruction_uncheckActions,

    bbInstruction_setTime,
    bbInstruction_unsetTime,

    //bbInstruction_netpauseButton,

    //bbInstruction_spawnEmptyEntity,
    //bbInstruction_unspawnEmptyEntity,

    //bbInstruction_spawnTestEntity,
    //bbInstruction_unspawnTestEntity,

    //bbInstruction_deleteEntity,
    //bbInstruction_undeleteEntity,

    //bbInstruction_entity_setComponent,
    //bbInstruction_entity_unsetComponent,


    //bbInstruction_setServerEntity,
    //bbInstruction_unsetServerEntity,

    //bbInstruction_spawnEntityOut,

    //bbInstruction_spawnGraphicsComponent,
    //bbInstruction_unspawnGraphicsComponent,

    //bbInstruction_updateMoveables,
    //bbInstruction_unupdateMoveables,

    //bbInstruction_spawnTestMoveable,
    //bbInstruction_unspawnTestMoveable,

    bbInstruction_numTypes
} bbInstruction_type;


typedef enum
{
    ///The instruction was added from inside the core
    bbInstructionSource_internal,
    ///The instruction was added in the MAIN thread
    bbInstructionSource_input,
    ///The instruction was added through the action queue
    bbInstructionSource_action,
    ///The instruction was called before the start of play
    bbInstructionSource_norewind,
} bbInstruction_source;

typedef struct
{
    bbMapCoords goalpoint;
    I32 entity;
} bbInstructionsData_goalPoint;

typedef struct
{
    bbMapCoords position;
    bbMapCoords goalpoint;
    char key[KEY_LENGTH];
} bbInstructionsData_spawnUnitOut;

//Untidy, but flexible
typedef struct
{
    bbHandle handle1;
    bbHandle handle2;
    bbHandle handle3;
} bbInstructionsData_threeHandles;

typedef struct
{
    I32 entity_int;
    bbHandle agent;
    bbHandle movable;
    bbHandle unit;
} bbInstructionsData_entity;

typedef struct
{
    bbHandle agent;
    bbSquareCoords square_coords;
} bbInstructionsData_agentSquare;

typedef struct
{
    bbHandle handle1;
    bbHandle handle2;
    bbMapCoords coords;
} bbInstructionsData_agentMapCoords;

typedef struct
{
    bbHandle agent;
    I32 hitpoints;
} bbInstructionsData_damageAgent;

typedef struct
{
    bbMapCoords coords;
    I32 entity;
    I32 button;
} bbInstructionsData_mapClick;

typedef struct
{
    I32 type;
    bbMapCoords position;
    bbMapCoords goalpoint;
    I32 entity;
    I32 movable;
} bbInstructionsData_spawnUnit;

typedef struct
{
    I32 type;
    bbMapCoords position;
    bbMapCoords goalpoint;
} bbInstructionsData_spawnAgent;

typedef struct
{
    I32 entities_available;
    I32 movables_available;
    I32 entity_int;
    I32 movable_int;
} bbInstructionsData_unspawnAgent;

// typedef struct
// {
//     bbMovable_type type;
//     I32 movable;
//     I32 goal_movable;
//     bbMapCoords goal_coords;
//     U64 last_state_change;
// } bbInstructionsData_goalMovable;

typedef union
{
    char key[KEY_LENGTH];
    U64 u64;
    bbMapCoords map_coords;
    bbInstructionsData_goalPoint goalpoint;
    bbInstructionsData_mapClick map_click;
    bbInstructionsData_agentSquare agent_square;
    bbInstructionsData_agentMapCoords agent_MC;
    bbInstructionsData_spawnUnit spawn_unit;
    bbInstructionsData_damageAgent damage_agent;
    bbInstructionsData_threeHandles three_handles;
    bbInstructionsData_spawnUnitOut spawn_unit_out;
    bbInstructionsData_spawnAgent spawn_agent;
    bbInstructionsData_unspawnAgent unspawn_agent;
    bbInstructionsData_entity entity;
} bbInstruction_data;

typedef struct bbInstruction
{
    I32 type;
    bbInstruction_data data;
    U64 act_time;
    U32 collision;
    U8 player;
    void* ECS;
    bbListElement_Handle list_element;
    bbHandle redo_instruction;
    bbInstruction_source source;
    bbHandle snapshot;
} bbInstruction;


bbFlag bbInstruction_checkActions_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_uncheckActions_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbInstruction_checkActions_fn(bbCore* core, bbInstruction* instruction);
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
bbFlag bbInstruction_unsetGoalpoint_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_approachGoalpoint_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unapproachGoalpoint_fn(bbCore* core, bbInstruction* instruction);


bbFlag bbInstruction_updateMoveables_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unupdateMoveables_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbInstruction_spawnTestMoveable_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unspawnTestMoveable_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbInstruction_spawnBananaOut_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_spawnBananaIn_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unspawnBanana_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbInstruction_testClick_fn(bbCore* core, bbInstruction* instruction);
#endif //BB_INSTRUCTION_H