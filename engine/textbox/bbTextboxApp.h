#ifndef BB_TEXTBOXAPP_H
#define BB_TEXTBOXAPP_H

#include "bbTextbox.h"

typedef enum
{
    bbTextbox_Dialogue,
    bbTextbox_Prompt,
    bbTextbox_numTextboxes,
} bbTextbox_Names;


typedef struct
{
    bbTextbox_system textbox_system;
    bbTextbox** textboxes;
} bbTextboxApp;

bbFlag bbTextboxApp_init(bbTextboxApp* app);

#endif //BB_TEXTBOXAPP_H