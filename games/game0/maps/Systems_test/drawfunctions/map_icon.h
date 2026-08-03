
bbFlag bbDF_mapiconTest(void* drawable, void* frameDescriptor, void* cl){
    bbDrawable* mapicon = drawable;
    drawFuncClosure* foo = cl;
    bbFrame* frame_descriptor = frameDescriptor;


    I32 spriteInt = frame_descriptor->handle.u64;

    bbGraphicsApp* graphics = foo->graphics;
    sfSprite* sprite = graphics->sprites->sprites[spriteInt];


    bbViewport* VP = foo->target;

    sfRenderTexture* renderTexture = VP->mapicon.renderTexture;

    sfVector2f V2F = bbMapCoords_getV2f_mapicon(mapicon->coords, VP);



    sfSprite_setPosition(sprite, V2F);
    sfRenderTexture_drawSprite(renderTexture, sprite, NULL);

    return bbSuccess;

}