#include "engine/data/bbHome.h"
#include "engine/geometry/bbViewportCoords.h"
#include "engine/graphics/bbGraphicsApp.h"
#include "engine/viewport/bbDrawBuffer.h"
#include "engine/viewport/bbViewport.h"

bbFlag bbDrawBuffer_bbHere(void* node, void* cl) {
    bbDebug("bbDrawBuffer_bbHere()\n")
    printf("####################################################\n");
    return bbSuccess;
}


bbFlag bbDB_sprite(void* node, void* cl){
    bbDrawBufferObject* object = node;
    drawBufferClosure* foo = cl;
    bbGraphicsApp* graphics = foo->graphics;
    bbViewport* VP = foo->target;


    U64 spriteInt = object->asset_handle.u64;
    if (spriteInt >= graphics->sprites->num_sprites) spriteInt = 0;
    sfSprite* sprite = graphics->sprites->sprites[spriteInt].sprite;
    sfRenderTexture* renderTexture = VP->main.renderTexture;
    sfVector2f V2F = bbMapCoords_getV2f(object->MC, VP);
    sfSprite_setPosition(sprite,V2F);
    sfRenderTexture_drawSprite(renderTexture,sprite,NULL);
    return bbSuccess;
}


bbFlag bbDB_unitSprite(void* node, void* cl){
    bbDrawBufferObject* object = node;
    drawBufferClosure* foo = cl;
    bbGraphicsApp* graphics = foo->graphics;
    bbViewport* VP = foo->target;

    U64 spriteInt = object->asset_handle.u64;
    if (spriteInt >= graphics->sprites->num_sprites) spriteInt = 0;
    sfSprite* sprite = graphics->sprites->sprites[spriteInt].sprite;
    sfVector2f V2F = bbMapCoords_getV2f(object->MC, VP);
    sfSprite_setPosition(sprite,V2F);

    sfRenderTexture* renderTexture = VP->main.renderTexture;
    sfRenderTexture_drawSprite(renderTexture,sprite,NULL);
    renderTexture = VP->highlight.renderTexture;
    sfRenderTexture_drawSprite(renderTexture,sprite,NULL);
    return bbSuccess;
}




bbFlag bbDrawBufferFunctions_populate(bbDrawBufferFunctions* draw_buffer) {
    bbDrawBufferFunctions_addFunction(draw_buffer,"DRAWBUFFER_HERE", bbDrawBuffer_bbHere);
    bbDrawBufferFunctions_addFunction(draw_buffer,"DRAWBUFFER_SPRITE", bbDB_sprite);
    bbDrawBufferFunctions_addFunction(draw_buffer,"DRAWBUFFER_UNITSPRITE", bbDB_unitSprite);

    return bbSuccess;
}
