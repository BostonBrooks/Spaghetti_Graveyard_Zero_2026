
#include <stdlib.h>

#include "engine/graphics/bbDrawfunctions.h"
#include "engine/logic/bbPrime.h"
#include "engine/logic/bbTerminal.h"
#include "engine/userinterface/bbWidgets.h"

bbFlag bbDF_NULL(void* drawable, void* frameDescriptor, void* cl)
{
    bbHere()
}

//Draw a sprite belonging to a widget
bbFlag bbDF_widgetSprite(void* drawable, void* frame_descriptor, void* cl){
    bbWidget* widget = drawable;
    bbFrame* frame = frame_descriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;

    I32 spriteInt = frame->handle.u64;
    sfSprite* sprite = graphics->sprites->sprites[spriteInt];


    bbScreenPoints SP;
    SP.x = widget->rect.left + frame->offset.x;
    SP.y = widget->rect.top + frame->offset.y;

    sfVector2f position = bbScreenPoints_getV2f(SP);
    sfSprite_setPosition(sprite, position);


    sfRenderWindow_drawSprite(closure->target, sprite, NULL);

    return bbSuccess;
}


// Draw an animation belonging to a widget;
bbFlag bbDF_widgetAnimation(void* drawable, void* frameDescriptor, void* cl){

    bbWidget* widget = drawable;
    bbFrame* frame_descriptor = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;

    bbAnimation* animation = graphics->animations->animations[frame_descriptor->handle.u64];

    I32 angle = 0;
    I32 frames = animation->frames;


    //bbDebug("key = %s, maptime = %d, starttime= %d, framerate = %f, frames = %d\n",
    //		animation->key, mapTime, frame_descriptor->startTime,animation->framerate, animation->frames );
    I32 frame = (int)((double)(closure->GUI_time - frame_descriptor->start_time) *
                      (double)animation->framerate * frame_descriptor->framerate) % animation->frames;
    I32 sprite_int = animation->Sprites[angle*frames+frame].u64;
    sfSprite* sprite = animation->sprites->sprites[sprite_int];



    bbScreenPoints SP;
    SP.x = widget->rect.left + frame_descriptor->offset.x;
    SP.y = widget->rect.top + frame_descriptor->offset.y;
    sfVector2f position = bbScreenPoints_getV2f(SP);

    sfSprite_setPosition(sprite, position);
    sfRenderWindow_drawSprite(closure->target, sprite, NULL);

    return bbSuccess;
}

bbFlag bbDF_widgetTextBox(void* drawable, void* frameDescriptor, void* cl)
{

    bbWidget* widget = drawable;
    bbFrame* frame = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;

    sfRenderWindow_drawText(closure->target, widget->type_data.text_box.text, NULL);
    return bbSuccess;

}

#define NUM_DRAWFUNCTIONS 4
bbFlag bbDrawfunctions_new(bbDrawfunctions** drawfunctions){

    bbDrawfunctions* functions = malloc(sizeof(bbDrawfunctions) + NUM_DRAWFUNCTIONS * sizeof(bbDrawFunction*));
    bbAssert(functions!=NULL, "bad malloc");
    bbDictionary_new(&functions->dictionary, nextPrime(NUM_DRAWFUNCTIONS));

    functions->num = NUM_DRAWFUNCTIONS;

    bbHandle handle;

    functions->functions[0] = bbDF_NULL;
    handle.u64 = 0;
    bbDictionary_add(functions->dictionary, "TEST", handle);

    functions->functions[1] = bbDF_widgetSprite;
    handle.u64 = 1;
    bbDictionary_add(functions->dictionary, "WIDGET_SPRITE", handle);

    functions->functions[2] = bbDF_widgetAnimation;
    handle.u64 = 2;
    bbDictionary_add(functions->dictionary, "WIDGET_ANIMATION", handle);

    functions->functions[3] = bbDF_widgetTextBox;
    handle.u64 = 3;
    bbDictionary_add(functions->dictionary, "TEXTBOX", handle);

    *drawfunctions = functions;
    return bbSuccess;
}
