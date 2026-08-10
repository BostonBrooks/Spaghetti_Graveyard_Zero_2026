#include "engine/geometry/bbViewportCoords.h"

bbFlag bbDF_eyeCandyTest(void* drawable, void* frameDescriptor, void* cl){
    bbDrawable* mapicon = drawable;
    drawFuncClosure* foo = cl;

    //bbDebug("eye candy\n", mapicon->label);
    I32 animationInt = 7;

    bbGraphicsApp* graphics = foo->graphics;
    I32 time = foo->map_time;
    bbAnimation* animation = graphics->animations->animations[animationInt];
    I32 frames = animation->frames;
    I32 frame = time % frames;
    I32 spriteInt = animation->Sprites[frame].u64;
    bbSprites* sprites = animation->sprites;
    sfSprite* sprite = sprites->sprites[spriteInt];


    bbViewport* VP = foo->target;

    sfRenderTexture* renderTexture = VP->main.renderTexture;

    sfVector2f V2F = bbMapCoords_getV2f(mapicon->coords, VP);



    sfSprite_setPosition(sprite, V2F);
    sfRenderTexture_drawSprite(renderTexture, sprite, NULL);

    return bbSuccess;

}
