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
/*
bbFlag bbDF_drawableAnimation(void* Drawable, void* frameDescriptor, void* cl){
    bbDrawable* drawable = Drawable;
    bbFrame* frame = frameDescriptor;
    drawFuncClosure* foo = cl;
    bbGraphicsApp* graphics = foo->graphics;
    bbViewport* VP = foo->target;

    I32 animationInt = frame->handle.u64;
    bbAnimation* animation = graphics->animations->animations[animationInt];

    //TODO - angle doesnt have to be hard-wired
    I32 angle = 0;
    I32 frames = animation->frames;

    I32 frameInt = (I64)((double)(foo->map_time - frame->start_time)
            *(double)animation->framerate*(double)frame->framerate) % frames;

    I32 spriteInt = animation->Sprites[angle*frames+frameInt].u64;

    if (animationInt == 9){ bbDebug("spriteInt = %d\n", spriteInt);}

    sfSprite* sprite = animation->sprites->sprites[spriteInt];
    sfRenderTexture* renderTexture = VP->main.renderTexture;

    sfVector2f V2F = bbMapCoords_getV2f(drawable->coords, VP);

    sfSprite_setPosition(sprite,V2F);

    sfRenderTexture_drawSprite(renderTexture,sprite,NULL);

    return bbSuccess;


}
*/
I32 getAngleXD(float radians, I32 numAngles)
{

    //TODO if(numAngles = 2) {left or right, 0 or 1}

    I32 angle = ((I32)((radians*0.15915494309f+1)*numAngles + numAngles/2.f - 0.5f))%numAngles;
    //bbDebug("radians/2pi = %f, getAngle = %d\n",radians*0.15915494309f, angle);
    return angle;

}

bbFlag bbDF_unitAnimationWAngle(void* Drawable, void* frameDescriptor, void* cl){
    bbDrawable* drawable = Drawable;
    bbFrame* frame = frameDescriptor;
    drawFuncClosure* foo = cl;
    bbGraphicsApp* graphics = foo->graphics;
    bbViewport* VP = foo->target;

    I32 animationInt = frame->handle.u64;
    bbAnimation* animation = graphics->animations->animations[animationInt];

//TODO may vary
    I32 numAngles = animation->angles;
    I32 angle = getAngleXD(drawable->rotation, numAngles);

    I32 frames = animation->frames;

    I32 frameInt = (I64)((double)(foo->map_time - frame->start_time)
            *(double)animation->framerate*(double)frame->framerate) % frames;

    I32 spriteInt = animation->Sprites[angle*frames+frameInt].u64;



    bbAssert(spriteInt >= 0, "Array index out of bounds");
    sfSprite* sprite = animation->sprites->sprites[spriteInt];

    sfVector2f V2F = bbMapCoords_getV2f(drawable->coords, VP);
    sfSprite_setPosition(sprite,V2F);

    sfRenderTexture* renderTexture = VP->main.renderTexture;
    sfRenderTexture_drawSprite(renderTexture,sprite,NULL);

     renderTexture = VP->highlight.renderTexture;
    sfRenderTexture_drawSprite(renderTexture,sprite,NULL);

    return bbSuccess;


}

bbFlag bbDF_unitStillWAngle(void* Drawable, void* frameDescriptor, void* cl){
    bbDrawable* drawable = Drawable;
    bbFrame* frame = frameDescriptor;
    drawFuncClosure* foo = cl;
    bbGraphicsApp* graphics = foo->graphics;
    bbViewport* VP = foo->target;

    I32 animationInt = frame->handle.u64;
    bbAnimation* animation = graphics->animations->animations[animationInt];

    //TODO may vary
    I32 numAngles = animation->angles;
    I32 angle = getAngleXD(drawable->rotation, numAngles);

    I32 frames = animation->frames;

    I32 frameInt = 0;

    I32 spriteInt = animation->Sprites[angle*frames+frameInt].u64;


    bbAssert(spriteInt >= 0, "Array index out of bounds");
    sfSprite* sprite = animation->sprites->sprites[spriteInt];

    sfVector2f V2F = bbMapCoords_getV2f(drawable->coords, VP);
    sfSprite_setPosition(sprite,V2F);

    sfRenderTexture* renderTexture = VP->main.renderTexture;
    sfRenderTexture_drawSprite(renderTexture,sprite,NULL);

    renderTexture = VP->highlight.renderTexture;
    sfRenderTexture_drawSprite(renderTexture,sprite,NULL);

    return bbSuccess;


}