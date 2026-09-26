#include "engine/userinterface/bbTextInput.h"

bbFlag bbDF_widgetTextInput(void* Drawable, void* frameDescriptor, void* cl)
{
    bbWidget* widget = Drawable;
    bbFrame* frame = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;

    bbTextInput* text = widget->extra_data;

    bbMutexLock(&text->mutex);
    sfText_setString(text->sf_text, text->bounded_buffer);
    bbMutexUnlock(&text->mutex);

    //bbDebug("spriteInt = %d, sprite = %p, target = %p, window = %p\n",
    //        spriteInt, sprite, target, home.private.window);
    bbScreenPoints SP;
    SP.x = widget->rect.left + frame->offset.x;
    SP.y = widget->rect.top + frame->offset.y;

    sfVector2f position = bbScreenPoints_getV2f(SP);
    sfText_setPosition(text->sf_text, position);


    sfRenderWindow_drawText(closure->target, text->sf_text, NULL);

    return bbSuccess;
}
