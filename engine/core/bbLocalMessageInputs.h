#ifndef BB_LOCAL_MESSAGE_INPUTS_H
#define BB_LOCAL_MESSAGE_INPUTS_H
#include "bbCore.h"


bbFlag bbLocalMessage_SetString(bbCore* core, char* string);

bbFlag bbLocalMessage_UnfreezeButton(bbCore* core, char* string);

bbFlag bbLocalMessage_ActionUnfreeze(bbCore* core, char* string);
bbFlag bbLocalMessage_RetroactionUnfreeze(bbCore* core, char* string);

bbFlag bbLocalMessage_NetsendButton(bbCore* core, char* key);


bbFlag bbLocalMessage_NetcodeButton(bbCore* core, char* key);


bbFlag bbLocalMessage_ActionLoop(bbCore* core, char* key);

bbFlag bbLocalMessage_KeyUp(bbCore* core, I32 key);
bbFlag bbLocalMessage_KeyDown(bbCore* core, I32 key);


bbFlag bbLocalMessage_NetpauseButton(bbCore* core, char* key);


bbFlag bbLocalMessage_SetViewpoint(bbCore* core, bbMapCoords mapCoords);
bbFlag bbLocalMessage_SetGoalpoint(bbCore* core, bbMapCoords mapCoords);
bbFlag bbLocalMessage_SwitchCharacterButton(bbCore* core, char* key);

bbFlag bbLocalMessage_SpawnBanana(bbCore* core, bbMapCoords mapCoords);
bbFlag bbLocalMessage_SpawnUnit(bbCore* core, bbMapCoords mapCoords, char* key);
#endif //BB_LOCAL_MESSAGE_INPUTS_H