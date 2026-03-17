
#include <stdlib.h>

#include "engine/graphics/bbDrawfunctions.h"
#include "engine/logic/bbPrime.h"
#include "engine/logic/bbTerminal.h"
#include "engine/userinterface/bbWidgets.h"
#include "games/game0/maps/map0/drawfunctions/textboxindicator.h"
#include "games/game0/maps/map0/drawfunctions/button_state.h"
#include "games/game0/maps/map0/drawfunctions/widget_text.h"

#define NUM_DRAWFUNCTIONS 12

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


    I32 frame = bbArith_mod((int)((double)(closure->GUI_time - frame_descriptor->start_time) *
                      (double)animation->framerate * frame_descriptor->framerate), animation->frames);
    I32 sprite_int = animation->Sprites[angle*frames+frame].u64;



    bbAssert(sprite_int <  graphics->sprites->num_sprites, "bad sprite int\n");
    sfSprite* sprite = animation->sprites->sprites[sprite_int];



    bbScreenPoints SP;
    SP.x = widget->rect.left + frame_descriptor->offset.x;
    SP.y = widget->rect.top + frame_descriptor->offset.y;
    sfVector2f position = bbScreenPoints_getV2f(SP);

    sfSprite_setPosition(sprite, position);
    sfRenderWindow_drawSprite(closure->target, sprite, NULL);

    return bbSuccess;
}

bbFlag bbDF_widgetAngle(void* drawable, void* frameDescriptor, void* cl){

    bbWidget* widget = drawable;
    bbFrame* frame_descriptor = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;

    bbAnimation* animation = graphics->animations->animations[frame_descriptor->handle.u64];

    I32 angle = widget->angle;
    I32 frames = animation->frames;


    I32 frame = 0;
    I32 sprite_int = animation->Sprites[angle*frames+frame].u64;



    bbAssert(sprite_int <  graphics->sprites->num_sprites, "bad sprite int\n");
    sfSprite* sprite = animation->sprites->sprites[sprite_int];



    bbScreenPoints SP;
    SP.x = widget->rect.left + frame_descriptor->offset.x;
    SP.y = widget->rect.top + frame_descriptor->offset.y;
    sfVector2f position = bbScreenPoints_getV2f(SP);

    sfSprite_setPosition(sprite, position);
    sfRenderWindow_drawSprite(closure->target, sprite, NULL);

    return bbSuccess;
}

bbFlag bbDF_widgetMapTimeAnimation(void* drawable, void* frameDescriptor, void* cl){

    bbWidget* widget = drawable;
    bbFrame* frame_descriptor = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;

    bbAnimation* animation = graphics->animations->animations[frame_descriptor->handle.u64];

    I32 angle = 0;
    I32 frames = animation->frames;

    double time = home.UI.clock2_handle.map_tick;

    //bbDebug("key = %s, maptime = %d, starttime= %d, framerate = %f, frames = %d\n",
    //		animation->key, mapTime, frame_descriptor->startTime,animation->framerate, animation->frames );
    I32 frame = (int)((double)(time - frame_descriptor->start_time) *
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

bbFlag bbDF_widgetServerTimeAnimation(void* drawable, void* frameDescriptor, void* cl){

    bbWidget* widget = drawable;
    bbFrame* frame_descriptor = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;

    bbAnimation* animation = graphics->animations->animations[frame_descriptor->handle.u64];

    I32 angle = 0;
    I32 frames = animation->frames;

    double time = home.UI.clock2_handle.server_tick;

    //bbDebug("key = %s, maptime = %d, starttime= %d, framerate = %f, frames = %d\n",
    //		animation->key, mapTime, frame_descriptor->startTime,animation->framerate, animation->frames );
    I32 frame = (int)((double)(time - frame_descriptor->start_time) *
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

//Look up default draw function for a given animation
bbFlag bbDF_widgetAnimationDefault(void* drawable, void* frameDescriptor, void* cl){
    bbWidget* widget = drawable;
    bbFrame* frame_descriptor = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;
    bbAnimation* animation = graphics->animations->animations[frame_descriptor->handle.u64];
    I32 drawFunctionInt = animation->drawfunction;
    bbDrawFunction *drawFunction = graphics->drawfunctions->functions[drawFunctionInt];
    return drawFunction(drawable, frame_descriptor, cl);
}

bbFlag bbDF_composition(void* drawable, void* frameDescriptor, void* cl){

    bbFrame* self_frame = frameDescriptor;
    drawFuncClosure* closure = cl;
    bbGraphicsApp* graphics = closure->graphics;
    bbComposition* composition = graphics->compositions->compositions[self_frame->handle.u64];
    bbFrame* input_frame;
    bbFrame output_frame;
    //void* output_object;

    //bbDebug("composition->num_frames = %d\n", composition->num_frames);
    for (int i = 0; i < composition->num_frames; i++){
        input_frame = &composition->frame[i];

        output_frame.type = input_frame->type;
        output_frame.handle = input_frame->handle;
        output_frame.offset.x = input_frame->offset.x + self_frame->offset.x;
        output_frame.offset.y = input_frame->offset.y + self_frame->offset.y;
        output_frame.framerate = input_frame->framerate * self_frame->framerate;
        output_frame.start_time = input_frame->start_time + self_frame->start_time;
        output_frame.drawfunction = input_frame->drawfunction;


        if (output_frame.drawfunction <0 || output_frame.drawfunction>= NUM_DRAWFUNCTIONS){
            bbHere()
        } else {

            bbDrawFunction *drawFunction =graphics->drawfunctions->functions[output_frame.drawfunction];
            drawFunction(drawable, &output_frame, cl);
        }
    }
    return bbSuccess;
}

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


    functions->functions[4] = bbDF_widgetMapTimeAnimation;
    handle.u64 = 4;
    bbDictionary_add(functions->dictionary, "WIDGET_MAPTIME_ANIMATION", handle);

    functions->functions[5] = bbDF_textboxIndicator;
    handle.u64 = 5;
    bbDictionary_add(functions->dictionary, "TEXTBOX_INDICATOR", handle);

    functions->functions[6] = bbDF_widgetAnimationDefault;
    handle.u64 = 6;
    bbDictionary_add(functions->dictionary, "ANIMATION_DEFAULT", handle);

    functions->functions[7] = bbDF_composition;
    handle.u64 = 7;
    bbDictionary_add(functions->dictionary, "COMPOSITION", handle);

    functions->functions[8] = bbDF_buttonState;
    handle.u64 = 8;
    bbDictionary_add(functions->dictionary, "BUTTON_STATE", handle);

    functions->functions[9] = bbDF_widgetText;
    handle.u64 = 9;
    bbDictionary_add(functions->dictionary, "WIDGET_TEXT", handle);

    functions->functions[10] = bbDF_widgetAngle;
    handle.u64 = 10;
    bbDictionary_add(functions->dictionary, "WIDGET_ANGLE", handle);


    functions->functions[11] = bbDF_widgetServerTimeAnimation;
    handle.u64 = 11;
    bbDictionary_add(functions->dictionary, "WIDGET_SERVERTIME_ANIMATION", handle);

    *drawfunctions = functions;
    return bbSuccess;
}
