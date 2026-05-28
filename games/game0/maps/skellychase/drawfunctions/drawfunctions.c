
#include <stdlib.h>

#include "engine/graphics/bbDrawfunctions.h"
#include "engine/logic/bbPrime.h"
#include "engine/logic/bbTerminal.h"
#include "engine/userinterface/bbWidgets.h"
#include "games/game0/maps/skellychase/drawfunctions/textbox_indicator.h"
#include "games/game0/maps/skellychase/drawfunctions/button_state.h"
#include "games/game0/maps/skellychase/drawfunctions/widget_text.h"
#include "games/game0/maps/skellychase/drawfunctions/clock.h"
#include "games/game0/maps/skellychase/drawfunctions/viewport.h"
#include "games/game0/maps/skellychase/drawfunctions/eye_candy.h"
#include "games/game0/maps/skellychase/drawfunctions/map_icon.h"
#include "games/game0/maps/skellychase/drawfunctions/drawable_animation.h"
#include "games/game0/maps/skellychase/drawfunctions/drawable_sprite.h"
#include "games/game0/maps/skellychase/drawfunctions/unit_animation.h"
#include "games/game0/maps/skellychase/drawfunctions/drawable_shadow.h"
#include "games/game0/maps/skellychase/drawfunctions/unit_sprite.h"
#include "games/game0/maps/skellychase/drawfunctions/compositions.h"
#include "games/game0/maps/skellychase/drawfunctions/health_bar.h"

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

bbFlag bbDF_none(void* drawable, void* frame_descriptor, void* cl){


    return bbSuccess;
}

bbFlag bbDrawfunctions_new(bbDrawfunctions** drawfunctions){

    bbDrawfunctions* functions = malloc(sizeof(bbDrawfunctions) + NUM_DRAWFUNCTIONS * sizeof(bbDrawFunction*));
    bbAssert(functions!=NULL, "bad malloc");
    bbDictionary_new(&functions->dictionary, nextPrime(NUM_DRAWFUNCTIONS));

    functions->num = NUM_DRAWFUNCTIONS;

    bbHandle handle;

    functions->functions[0] = bbDF_widgetSprite;
    handle.u64 = 0;
    bbDictionary_add(functions->dictionary, "WIDGET_SPRITE", handle);


    functions->functions[1] = bbDF_widgetGraphicsTimeAnimation;
    handle.u64 = 1;
    bbDictionary_add(functions->dictionary, "WIDGET_GRAPHICSTIME_ANIMATION", handle);

    functions->functions[2] = bbDF_widgetServerTimeAnimation;
    handle.u64 = 2;
    bbDictionary_add(functions->dictionary, "WIDGET_SERVERTIME_ANIMATION", handle);

    functions->functions[3] = bbDF_widgetCoreTimeAnimation;
    handle.u64 = 3;
    bbDictionary_add(functions->dictionary, "WIDGET_CORETIME_ANIMATION", handle);

    functions->functions[4] = bbDF_buttonState;
    handle.u64 = 4;
    bbDictionary_add(functions->dictionary, "BUTTON_STATE", handle);

    functions->functions[5] = bbDF_widgetText;
    handle.u64 = 5;
    bbDictionary_add(functions->dictionary, "WIDGET_TEXT", handle);

    functions->functions[6] = bbDF_widgetViewport;
    handle.u64 = 6;
    bbDictionary_add(functions->dictionary, "WIDGET_VIEWPORT", handle);

    functions->functions[7] = bbDF_eyeCandyTest;
    handle.u64 = 7;
    bbDictionary_add(functions->dictionary, "EYE_CANDY", handle);


    functions->functions[8] = bbDF_drawableAnimation;
    handle.u64 = 8;
    bbDictionary_add(functions->dictionary, "DRAWABLE_ANIMATION", handle);


    functions->functions[9] = bbDF_mapiconTest;
    handle.u64 = 9;
    bbDictionary_add(functions->dictionary, "MAPICON_TEST", handle);

    functions->functions[10] = bbDF_drawableSprite;
    handle.u64 = 10;
    bbDictionary_add(functions->dictionary, "DRAWABLE_SPRITE", handle);

    functions->functions[11] = bbDF_drawableAnimationWAngle;
    handle.u64 = 11;
    bbDictionary_add(functions->dictionary, "DRAWABLE_ANIMATION_ANGLE", handle);

    functions->functions[12] = bbDF_unitAnimationWAngle;
    handle.u64 = 12;
    bbDictionary_add(functions->dictionary, "UNIT_ANIMATION_ANGLE", handle);

    functions->functions[13] = bbDF_drawableShadow;
    handle.u64 = 13;
    bbDictionary_add(functions->dictionary, "DRAWABLE_SHADOW", handle);

    functions->functions[14] = bbDF_unitSprite;
    handle.u64 = 14;
    bbDictionary_add(functions->dictionary, "UNIT_SPRITE", handle);

    functions->functions[15] = bbDF_unitStillWAngle;
    handle.u64 = 15;
    bbDictionary_add(functions->dictionary, "UNIT_STILL_ANGLE", handle);

    functions->functions[16] = bbDF_composition;
    handle.u64 = 16;
    bbDictionary_add(functions->dictionary, "COMPOSITION", handle);

    functions->functions[17] = bbDF_compositionState;
    handle.u64 = 17;
    bbDictionary_add(functions->dictionary, "COMPOSITION_STATE", handle);

    functions->functions[18] = bbDF_none;
    handle.u64 = 18;
    bbDictionary_add(functions->dictionary, "NONE", handle);

    functions->functions[19] = bbDF_healthPointsTest;
    handle.u64 = 19;
    bbDictionary_add(functions->dictionary, "HEALTH_POINTS", handle);


    *drawfunctions = functions;
    return bbSuccess;
}

