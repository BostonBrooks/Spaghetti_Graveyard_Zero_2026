#include "engine/textbox/bbTextboxApp.h"



bbFlag bbTextboxApp_init(bbTextboxApp* app)
{
    app->textboxes = calloc(bbTextbox_numTextboxes, sizeof(bbTextbox*));
    bbTextbox_systemInit(&app->textbox_system);
    for (I32 i = 0; i < bbTextbox_numTextboxes; i++)
    {
        bbTextbox_new(&app->textboxes[i], &app->textbox_system, "UNUSED");
    }
    return bbSuccess;

}