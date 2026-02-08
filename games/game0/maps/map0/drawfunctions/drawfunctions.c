
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

#define NUM_DRAWFUNCTIONS 2
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

    *drawfunctions = functions;
    return bbSuccess;
}
