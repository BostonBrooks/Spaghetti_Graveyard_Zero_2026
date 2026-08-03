#include "engine/logic/bbFlag.h"
#include "engine/logic/bbIntTypes.h"
#include "engine/logic/bbDictionary.h"
#include "engine/logic/bbPrime.h"
#include "engine/graphics/bbDrawfunctions.h"
#include "engine/logic/bbTerminal.h"
#include "engine/userinterface/bbWidgets.h"
#include "engine/viewport/bbViewport.h"
#include "engine/viewport/bbDrawables.h"
#include "engine/viewport/bbMapIcons.h"
#include "engine/geometry/bbViewportCoords.h"
#include "engine/data/bbHome.h"

bbFlag bbDF_drawableSprite(void* Drawable, void* frameDescriptor, void* cl){
    bbDrawable* drawable = Drawable;
    bbFrame* frame = frameDescriptor;
    drawFuncClosure* foo = cl;
    bbGraphicsApp* graphics = foo->graphics;
    bbViewport* VP = foo->target;


    I32 spriteInt = frame->handle.u64;
    sfSprite* sprite = graphics->sprites->sprites[spriteInt];

    sfRenderTexture* renderTexture = VP->main.renderTexture;

    sfVector2f V2F = bbMapCoords_getV2f(drawable->coords, VP);

    sfSprite_setPosition(sprite,V2F);

    sfRenderTexture_drawSprite(renderTexture,sprite,NULL);


    return bbSuccess;


}