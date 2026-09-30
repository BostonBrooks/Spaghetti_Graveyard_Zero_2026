#ifndef BB_INPUTMODE_H
#define BB_INPUTMODE_H
///Different input modes eg send keystrokes to a textbox widget,
///use keys as control keys
///key combinations?

#include "engine/data/CSFML.h"

#include "bbWidgets.h"
#include "engine/core/bbCore.h"
#include "engine/core/bbInstruction.h"
#include "engine/logic/bbFlag.h"
#include "engine/logic/bbHandle.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/viewport/bbViewportMouseFunctions.h"

#define MAX_INPUT_MODES 16

typedef struct bbKeyAction bbKeyAction;

typedef struct bbInputMode bbInputMode;


typedef bbFlag bbKeyAction_fn (bbInputMode* input_mode, sfEvent * event, bbKeyAction* action);

typedef bbFlag bbClickEntity(bbInputMode* input_mode, bbHandle entity_handle, bbVPMouseType type);
typedef bbFlag bbClickMapCoords(bbInputMode* input_mode, bbMapCoords map_coords, bbVPMouseType type);

typedef struct bbKeyAction
{
    char lowercase;
    char uppercase;
    U8 control_key;
    U8 event_code;
    U8 event_code_release;
    bbKeyAction_fn* function;
} bbKeyAction;

typedef struct bbInputMode
{
    bbKeyAction_fn* default_function;
    U64 control_keys;
    bbWidget* widget;
    bbKeyAction key_actions[sfKeyCount];

    bbClickEntity* click_entity;
    bbClickMapCoords* click_map_coords;
} bbInputMode;

typedef struct
{
    I32 modes_available;
    bbDictionary* dict;
    bbInputMode* modes[MAX_INPUT_MODES];
    bbInputMode* current_mode;

} bbInputModes;
//typedef bbFlag key_action_fn (struct bbInput* input_object, sfEvent * event );
bbFlag bbInputModes_init(bbInputModes* input_modes);

bbFlag bbInputModes_add(bbInputModes* input_modes, bbInputMode* input_mode, char* key);
bbFlag bbInputModes_populate(bbInputModes* input_modes);
bbFlag bbInputModes_set(bbInputModes* input_modes, char* key);
bbFlag bbInputModes_event(bbInputModes* input_modes, sfEvent* event);
//bbFlag bbUI_Inbox_SetInputMode(bbUI_Inbox* inbox, bbHandle entity, I32 state);

bbFlag bbKeyAction_null(bbInputMode* input_mode, sfEvent * event, bbKeyAction* action);
bbFlag bbKeyAction_putChar(bbInputMode* input_mode, sfEvent * event, bbKeyAction* action);
bbFlag bbKeyAction_ctrl(bbInputMode* input_mode, sfEvent * event, bbKeyAction* action);
bbFlag bbKeyAction_event(bbInputMode* input_mode, sfEvent * event, bbKeyAction* action);
bbFlag bbKeyAction_putCharCtrl(bbInputMode* input_mode, sfEvent * event, bbKeyAction* action);
bbFlag bbKeyAction_eventCtrl(bbInputMode* input_mode, sfEvent * event, bbKeyAction* action);
bbFlag bbKeyAction_clearChar(bbInputMode* input_mode, sfEvent * event, bbKeyAction* action);


bbFlag bbClickEntity_print(bbInputMode* input_mode, bbHandle entity_handle, bbVPMouseType type);
bbFlag bbClickMapCoords_print(bbInputMode* input_mode, bbMapCoords map_coords, bbVPMouseType type);



#endif  //BB_INPUTMODE_H