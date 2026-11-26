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

bbFlag bbDF_projectileRotation(void* Drawable, void* frameDescriptor, void* cl){
    bbDrawable* drawable = Drawable;
    bbFrame* frame = frameDescriptor;
    drawFuncClosure* foo = cl;
    bbGraphicsApp* graphics = foo->graphics;
    bbViewport* VP = foo->target;
    bbUnit* unit = Drawable;

    I32 spriteInt = frame->asset_handle.u64;

    bbDebug("spriteInt = %d\n", spriteInt);
    sfSprite* sprite = graphics->sprites->sprites[spriteInt].sprite;

    bbDebug("sprite = %p\n", sprite);

    sfRenderTexture* renderTexture = VP->main.renderTexture;

    sfVector2f V2F = bbMapCoords_getV2f(drawable->md.coords, VP);

    sfSprite_setPosition(sprite,V2F);
    sfSprite_setRotation(sprite,unit->drawable.md.rotation  *180.0f / M_PI);

    bbDebug("rotation = %f\n", unit->drawable.md.rotation  *180.0f / M_PI);
    sfRenderTexture_drawSprite(renderTexture,sprite,NULL);

    return bbSuccess;


}

bbFlag bbDF_projectileRotationAngle(void* Drawable, void* frameDescriptor, void* cl){
    bbDrawable* drawable = Drawable;
    bbFrame* frame = frameDescriptor;
    drawFuncClosure* foo = cl;
    bbGraphicsApp* graphics = foo->graphics;
    bbViewport* VP = foo->target;
    bbUnit* unit = Drawable;


    I32 animationInt = frame->asset_handle.u64;
    bbAnimation* animation = graphics->animations->animations[animationInt];

    //TODO may vary
    I32 numAngles = animation->angles;
    I32 angle = getAngleXD(unit->drawable.md.angle, numAngles);

    I32 frames = animation->frames;

    I32 frameInt = 0;

    I32 spriteInt = animation->Sprites[angle*frames+frameInt].u64;


    bbAssert(spriteInt >= 0, "Array index out of bounds");
    sfSprite* sprite = animation->sprites->sprites[spriteInt].sprite;

    sfVector2f V2F = bbMapCoords_getV2f(drawable->md.coords, VP);

    V2F.x += frame->offset.x / (float)POINTS_PER_PIXEL;
    V2F.y += frame->offset.y / (float)POINTS_PER_PIXEL;
    sfSprite_setPosition(sprite,V2F);
    sfSprite_setRotation(sprite,unit->drawable.md.rotation  *180.0f / M_PI);

    sfRenderTexture* renderTexture = VP->main.renderTexture;
    sfRenderTexture_drawSprite(renderTexture,sprite,NULL);

    return bbSuccess;


}

bbFlag bbDF_projectiles(void* Drawable, void* frameDescriptor, void* cl) {
    bbRenderUnit* unit = Drawable;
    drawFuncClosure* foo = cl;
    bbGraphicsApp* graphics = foo->graphics;
    bbViewport* VP = foo->target;
    bbFrame* frame = frameDescriptor;

    if (unit->visible == false) {return bbSuccess;}

    I32 animationInt = frame->asset_handle.u64;
    bbAnimation* animation = graphics->animations->animations[animationInt];

    I32 numAngles = animation->angles;
    I32 angle = getAngleXD(unit->md.angle, numAngles);

    I32 frames = animation->frames;

    I32 frameInt = 0;

    I32 spriteInt = animation->Sprites[angle*frames+frameInt].u64;


    if (spriteInt >= graphics->sprites->num_sprites) spriteInt = 0;
    if (spriteInt < 0) spriteInt = 0;
    sfSprite* sprite = graphics->sprites->sprites[spriteInt].sprite;
    sfRenderTexture* renderTexture = VP->main.renderTexture;
    sfVector2f V2F = bbMapCoords_getV2f(unit->md.coords, VP);
    sfSprite_setPosition(sprite,V2F);
    sfSprite_setRotation(sprite,unit->md.rotation  *180.0f / M_PI);
    sfRenderTexture_drawSprite(renderTexture,sprite,NULL);

    return bbSuccess;
}
