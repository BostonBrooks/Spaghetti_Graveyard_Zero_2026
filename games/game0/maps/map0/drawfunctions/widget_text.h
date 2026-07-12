
#include "engine/data/CSFML.h"

#include "engine/graphics/bbCompositions.h"
#include "engine/graphics/bbDrawfunctions.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/graphics/bbSprites.h"
#include "engine/graphics/bbTextures.h"
#include "engine/logic/bbFlag.h"
#include "engine/userinterface/bbWidgetFunctions.h"

sfText* text;

bbFlag bbDF_widgetText(void* Drawable, void* frameDescriptor, void* cl){
        bbWidget* widget = Drawable;
        bbFrame* frame = frameDescriptor;
        drawFuncClosure* closure = cl;
        bbGraphicsApp* graphics = closure->graphics;

        if (text == NULL)
        {
#ifdef CSFML3
            text = sfText_create(graphics->fonts->fonts[0]);
            sfText_setCharacterSize(text, 12);
            sfText_setFillColor(text, sfBlack);

#endif
#ifndef CSFML3
            text = sfText_create();
            sfText_setFont(text, graphics->fonts->fonts[0]);
            sfText_setCharacterSize(text, 12);
            sfText_setColor(text, sfBlack);
#endif
        }
        sfText_setString(text, widget->key);

        //bbDebug("spriteInt = %d, sprite = %p, target = %p, window = %p\n",
        //        spriteInt, sprite, target, home.private.window);
        bbScreenPoints SP;
        SP.x = widget->rect.left + frame->offset.x;
        SP.y = widget->rect.top + frame->offset.y;

        sfVector2f position = bbScreenPoints_getV2f(SP);
        sfText_setPosition(text, position);


        sfRenderWindow_drawText(closure->target, text, NULL);

        return bbSuccess;
    }