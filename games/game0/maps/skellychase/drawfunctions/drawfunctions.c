
#include <stdlib.h>

#include "engine/graphics/bbDrawfunctions.h"
#include "engine/logic/bbPrime.h"
#include "engine/logic/bbTerminal.h"
#include "engine/userinterface/bbWidgets.h"
#include "games/game0/maps/skellychase/drawfunctions/textboxindicator.h"
#include "games/game0/maps/skellychase/drawfunctions/button_state.h"
#include "games/game0/maps/skellychase/drawfunctions/widget_text.h"
#include "games/game0/maps/skellychase/drawfunctions/clock.h"

#define NUM_DRAWFUNCTIONS 69

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

bbFlag bbDrawfunctions_new(bbDrawfunctions** drawfunctions){

    bbDrawfunctions* functions = malloc(sizeof(bbDrawfunctions) + NUM_DRAWFUNCTIONS * sizeof(bbDrawFunction*));
    bbAssert(functions!=NULL, "bad malloc");
    bbDictionary_new(&functions->dictionary, nextPrime(NUM_DRAWFUNCTIONS));

    functions->num = NUM_DRAWFUNCTIONS;

    bbHandle handle;

    functions->functions[1] = bbDF_widgetSprite;
    handle.u64 = 1;
    bbDictionary_add(functions->dictionary, "WIDGET_SPRITE", handle);


    functions->functions[4] = bbDF_widgetMapTimeAnimation;
    handle.u64 = 4;
    bbDictionary_add(functions->dictionary, "WIDGET_MAPTIME_ANIMATION", handle);

    functions->functions[11] = bbDF_widgetServerTimeAnimation;
    handle.u64 = 11;
    bbDictionary_add(functions->dictionary, "WIDGET_SERVERTIME_ANIMATION", handle);

    functions->functions[12] = bbDF_widgetCoreTimeAnimation;
    handle.u64 = 12;
    bbDictionary_add(functions->dictionary, "WIDGET_CORETIME_ANIMATION", handle);

    functions->functions[8] = bbDF_buttonState;
    handle.u64 = 8;
    bbDictionary_add(functions->dictionary, "BUTTON_STATE", handle);

    functions->functions[9] = bbDF_widgetText;
    handle.u64 = 9;
    bbDictionary_add(functions->dictionary, "WIDGET_TEXT", handle);

    *drawfunctions = functions;
    return bbSuccess;
}

