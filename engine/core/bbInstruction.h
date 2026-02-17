///bbInstruction is used to tell the core what to do

#ifndef BB_INSTRUCTION_H
#define BB_INSTRUCTION_H

#include "engine/logic/bbHandle.h"
#include "engine/logic/bbFlag.h"
#include "engine/core/bbCore.h"
#include "engine/data/bbConstants.h"

typedef enum
{
    bbInstruction_printInteger,
    bbInstruction_unprintInteger,

    bbInstruction_printString,
    bbInstruction_unprintString,

    bbInstruction_unfreezeButton,
    bbInstruction_unfreezeButton2,
    bbInstruction_netsendButton,
    bbInstruction_netcodeButton,
    bbInstruction_loopAction,

    bbInstruction_setQuote,
    bbInstruction_unsetQuote,

    bbInstruction_setTime,
    bbInstruction_unsetTime,

    bbInstruction_checkActions,
    bbInstruction_uncheckActions,


    bbInstruction_setTestInt,
    bbInstruction_unsetTestInt
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
    I64 integer;
} bbInstructionData_integer;

typedef struct
{
    char string[KEY_LENGTH];
} bbInstructionData_string;

typedef union
{
    bbInstructionData_integer integer;
    bbInstructionData_string string;
} bbInstruction_data;

typedef struct
{
    bbInstruction_type type;
    bbInstruction_data data;
    U64 act_time;
    bbListElement_Handle list_element;
    bbHandle redo_instruction;

    //TODO instead of bool is_input, use redo!=NULL
    bool is_input;
    bool is_action;

    bbInstruction_source source;
} bbInstruction;

///(4) instruction declaration
bbFlag bbInstruction_setQuote_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unsetQuote_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbInstruction_setTime_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unsetTime_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbInstruction_checkActions_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_uncheckActions_fn(bbCore* core, bbInstruction* instruction);



bbFlag bbInstruction_setTestInt_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unsetTestInt_fn(bbCore* core, bbInstruction* instruction);



//the following are too cludgey and will be deprecated
bbFlag bbInstruction_printInteger_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unprintInteger_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_printString_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unprintString_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unfreezeButton_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_unfreezeButton2_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_netsendButton_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_netcodeButton_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbInstruction_loopAction_fn(bbCore* core, bbInstruction* instruction);
#endif // BB_INSTRUCTION_H