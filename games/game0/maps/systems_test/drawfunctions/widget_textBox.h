#include "engine/data/bbHome.h"
#include "engine/data/CSFML.h"

#include "engine/graphics/bbCompositions.h"
#include "engine/graphics/bbDrawfunctions.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/graphics/bbSprites.h"
#include "engine/graphics/bbTextures.h"
#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbWidgetFunctions.h"
#include "engine/userinterface/bbWidgets.h"
#include "widgets/bbWidget_textboxData.h"

bbFlag bbDF_widgetTextBox(void* Drawable, void* frameDescriptor, void* cl)
{
    bbWidget* widget = Drawable;
    bbFrame* frame = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;

    bbWidget_textboxData* extra_data = widget->extra_data;
    if (extra_data->textbox == NULL)
    {
        bbHandle textbox_handle;
        bbDictionary_lookup(home.textbox_system.dict, widget->key,
                            &textbox_handle);
        extra_data->textbox = textbox_handle.ptr;
    }
    bbTextbox_copyBuffer(extra_data->textbox, extra_data->text, 60, 24,
                         MESSAGE_BUFFER_LENGTH);
    sfText_setString(extra_data->sf_text, extra_data->text);

    //bbDebug("spriteInt = %d, sprite = %p, target = %p, window = %p\n",
    //        spriteInt, sprite, target, home.private.window);
    bbScreenPoints SP;
    SP.x = widget->rect.left + frame->offset.x;
    SP.y = widget->rect.top + frame->offset.y;

    sfVector2f position = bbScreenPoints_getV2f(SP);
    sfText_setPosition(extra_data->sf_text, position);


    sfRenderWindow_drawText(closure->target, extra_data->sf_text, NULL);

    return bbSuccess;
}
