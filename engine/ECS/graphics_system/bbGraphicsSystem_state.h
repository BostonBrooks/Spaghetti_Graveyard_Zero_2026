
#include "bbGraphicsSystem.h"
typedef struct bbInstruction bbInstruction;

bbFlag bbUI_Inbox_SetEntityState2(bbUI_Inbox* inbox, bbGraphicsComponent_data* data);

bbFlag bbCI_Graphics_setState(bbCore* core, bbHandle entity_handle, I32 state, bbTime time, bbInstruction_source source, bbHandle action);

bbFlag bbI_Graphics_setState_fn(bbCore* core, bbInstruction* instruction);
bbFlag bbI_Graphics_unsetState_fn(bbCore* core, bbInstruction* instruction);

bbFlag bbGraphics_updateState(bbGraphicsComponent_data* new, bbGraphicsComponent_data* old, I32 state, bbTime time);